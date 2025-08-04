

#include "mee.hpp"

mee::common::num::num(double n) noexcept : value(n)
{
}

mee::common::num::~num() = default;

double mee::common::num::get_value() const
{
    return this->value;
}

mee::common::op::op(optype ot) noexcept : otype(ot)
{
}

mee::common::op::op() noexcept : otype(mee::common::optype::unspecified)
{
}

mee::common::op::~op() = default;

mee::common::optype mee::common::op::get_type() const noexcept
{
    return this->otype;
}

mee::tddp::expr::expr(std::vector<mee::common::node> n) : nodes(std::move(n)), index(0)
{
}

mee::tddp::expr::~expr() = default;

mee::common::node mee::tddp::expr::consume_node()
{
    return this->nodes.at(this->index++);
}

bool mee::tddp::expr::exists_node() const
{
    return (this->index < this->nodes.size());
}

constexpr int mee::tddp::expr::precedence(mee::common::optype o) const noexcept
{
    switch (o)
    {
        case mee::common::optype::add:
        case mee::common::optype::sub:
        {
            return 10;
        }
        case mee::common::optype::mul:
        case mee::common::optype::div:
        {
            return 20;
        }
        default: 
        {
            break;
        }
    }

    return 0;
}

double mee::tddp::expr::parse(int mp)
{
    double l = 0;

    if (auto v = std::get_if<mee::common::num>(&this->get_current()))
    {
        this->consume_node();
        l = v->get_value();
    }
    else if (auto v = std::get_if<mee::common::op>(&this->get_current()))
    {
        switch (v->get_type())
        {
            case mee::common::optype::lp:
            {
                this->consume_node();
                l = this->parse();

                if (auto rv = std::get_if<mee::common::op>(&this->get_current()))
                {
                    switch (rv->get_type())
                    {
                        case mee::common::optype::rp:
                        {
                            this->consume_node();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                }
            }
            default:
            {
                break;
            }
        }
    }

    while (this->exists_node())
    {
        if (auto v = std::get_if<mee::common::op>(&this->get_current()))
        {
            mee::common::optype o = v->get_type();
            if (o == mee::common::optype::rp) break;

            int lp = this->precedence(o);
            if (lp < mp) 
            {
                break;
            }

            this->consume_node();
            int rp = lp + 1;

            double r = this->parse(rp);
            switch (o)
            {
                case mee::common::optype::add:
                {
                    l += r;
                    break;
                }
                case mee::common::optype::sub:
                {
                    l -= r;
                    break;
                }
                case mee::common::optype::mul:
                {
                    l *= r;
                    break;
                }
                case mee::common::optype::div:
                {
                    if (r == 0)
                    {
                        throw std::runtime_error("division by zero");
                    }
                    l /= r;
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }

    return l;
}

const std::vector<mee::common::node> &mee::tddp::expr::get_nodes() const
{
    return this->nodes;
}

const mee::common::node &mee::tddp::expr::get_current() const
{
    return this->nodes.at(this->index);
}

std::size_t mee::tddp::expr::get_index() const noexcept
{
    return this->index;
}

mee::rdp::expr::expr(std::vector<mee::common::node> n) : nodes(std::move(n)), index(0)
{
}

mee::rdp::expr::~expr() = default;

mee::common::node mee::rdp::expr::consume_node()
{
    return this->nodes.at(this->index++);
}

bool mee::rdp::expr::exists_node() const
{
    return (this->index < this->nodes.size());
}

double mee::rdp::expr::parse_expr()
{
    double l = this->parse_term();

    while (this->exists_node())
    {
        if (auto v = std::get_if<mee::common::op>(&this->get_current()))
        {
            if (v->get_type() == mee::common::optype::add)
            {
                this->consume_node();
                l += this->parse_term();
            }
            else if (v->get_type() == mee::common::optype::sub)
            {
                this->consume_node();
                l -= this->parse_term();                
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return l;
}

double mee::rdp::expr::parse_term()
{
    double l = this->parse_factor();

    while (this->exists_node())
    {
        if (auto v = std::get_if<mee::common::op>(&this->get_current()))
        {
            if (v->get_type() == mee::common::optype::mul)
            {
                this->consume_node();
                l *= this->parse_factor();
            }
            else if (v->get_type() == mee::common::optype::div)
            {
                this->consume_node();
                l /= this->parse_factor();                
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return l;
}

double mee::rdp::expr::parse_factor()
{
    if (!this->exists_node())
    {
        throw std::runtime_error("invalid node count");
    }

    if (auto v = std::get_if<mee::common::num>(&this->get_current()))
    {
        this->consume_node();
        return v->get_value();
    }

    if (auto v = std::get_if<mee::common::op>(&this->get_current()))
    {
        switch (v->get_type())
        {
            case mee::common::optype::lp:
            {
                this->consume_node();
                double e = this->parse_expr();

                if (auto rv = std::get_if<mee::common::op>(&this->get_current()))
                {
                    switch (rv->get_type())
                    {
                        case mee::common::optype::rp:
                        {
                            this->consume_node();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                }

                return e;
            }
            default:
            {
                break;
            }
        }
    }

    return 0;
}

const std::vector<mee::common::node> &mee::rdp::expr::get_nodes() const
{
    return this->nodes;
}

const mee::common::node &mee::rdp::expr::get_current() const
{
    return this->nodes.at(this->index);
}

std::size_t mee::rdp::expr::get_index() const noexcept
{
    return this->index;
}

mee::common::lexer::lexer(std::string c) : nodes(), content(std::move(c)), ccindex(0)
{
}

mee::common::lexer::~lexer() = default;

char mee::common::lexer::current_char() const noexcept
{
    return this->content.at(this->ccindex);
}

char mee::common::lexer::consume_char() noexcept
{
    return this->content.at(this->ccindex++);
}

bool mee::common::lexer::exists_char() const noexcept
{
    return (this->ccindex < this->content.size());
}

const std::vector<mee::common::node> &mee::common::lexer::get_nodes() const
{
    return this->nodes;
}

const std::string &mee::common::lexer::get_content() const
{
    return this->content;
}

std::size_t mee::common::lexer::get_ccindex() const noexcept
{
    return this->ccindex;
}

void mee::common::lexer::tokenize()
{
    while (true)
    {
        if (!this->exists_char())
        {
            break;
        }

        char c = this->current_char();

        if (std::isspace(c))
        {
            this->consume_char();
            continue;
        }
        if (c == '(')
        {
            this->nodes.emplace_back(mee::common::op(mee::common::optype::lp));
            this->consume_char();
            continue;
        }
        if (c == ')')
        {
            this->nodes.emplace_back(mee::common::op(mee::common::optype::rp));
            this->consume_char();
            continue;
        }
        if (c == '+')
        {
            this->nodes.emplace_back(mee::common::op(mee::common::optype::add));
            this->consume_char();            
            continue;
        }
        if (c == '-')
        {
            this->nodes.emplace_back(mee::common::op(mee::common::optype::sub));
            this->consume_char();            
            continue;
        }
        if (c == '*')
        {
            this->nodes.emplace_back(mee::common::op(mee::common::optype::mul));
            this->consume_char();            
            continue;
        }
        if (c == '/')
        {
            this->nodes.emplace_back(mee::common::op(mee::common::optype::div));
            this->consume_char();            
            continue;
        }
        if (std::isdigit(c))
        {
            std::string vs;
            double v = 0;

            while (this->exists_char() && std::isdigit(this->current_char()))
            {
                vs.push_back(this->current_char());
                this->consume_char();
            }

            if (this->exists_char() && this->current_char() == '.')
            {
                vs.push_back(this->current_char());
                this->consume_char();
                while (this->exists_char() && std::isdigit(this->current_char()))
                {
                    vs.push_back(this->current_char());
                    this->consume_char();
                }
            }
            v = std::stod(vs);
            this->nodes.emplace_back(mee::common::num(v));
            continue;
        }
        if (c == ';')
        {
            this->consume_char();
            continue;
        }
    }    
}
