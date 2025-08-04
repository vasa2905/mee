

#ifndef _MEE_HPP
#define _MEE_HPP

#include <vector>
#include <variant>
#include <string>
#include <stdexcept>

namespace mee
{
    namespace common
    {
        enum class optype
        {
            unspecified,
            add,
            sub,
            mul,
            div,
            lp,
            rp
        };

        class num 
        {
            private:
                double value;

            public:
                explicit num(double n) noexcept;
                ~num();

                [[nodiscard]] double get_value() const;
        };

        class op 
        {
            private:
                optype otype;
            
            public:
                op(optype ot) noexcept;
                op() noexcept;
                ~op();

                [[nodiscard]] optype get_type() const noexcept;
        };

        using node = std::variant<std::monostate, num, op>;

        class lexer
        {
            private:
                std::vector<node> nodes;
                std::string content;
                std::size_t ccindex;

                [[nodiscard]] char current_char() const noexcept;
                char consume_char() noexcept;
                [[nodiscard]] bool exists_char() const noexcept;

            public:
                explicit lexer(std::string c);
                ~lexer();

                void tokenize();

                [[nodiscard]] const std::vector<node> &get_nodes() const;
                [[nodiscard]] const std::string &get_content() const;
                [[nodiscard]] std::size_t get_ccindex() const noexcept;
        };
    }

    namespace tddp
    {
        class expr 
        {
            private:
                std::vector<mee::common::node> nodes;
                std::size_t index;

                mee::common::node consume_node();
                [[nodiscard]] constexpr int precedence(mee::common::optype o) const noexcept;
                [[nodiscard]] bool exists_node() const;

            public:
                explicit expr(std::vector<mee::common::node> n);
                ~expr();

                [[nodiscard]] double parse(int mp = 0);

                [[nodiscard]] const std::vector<mee::common::node> &get_nodes() const;
                [[nodiscard]] const mee::common::node &get_current() const;
                [[nodiscard]] std::size_t get_index() const noexcept;
        };
    }

    namespace rdp
    {
        class expr
        {
            private:
                std::vector<mee::common::node> nodes;
                std::size_t index;

                mee::common::node consume_node();
                [[nodiscard]] bool exists_node() const;

            public:
                explicit expr(std::vector<mee::common::node> n);
                ~expr();
                
                double parse_expr();
                double parse_term();
                double parse_factor();

                [[nodiscard]] const std::vector<mee::common::node> &get_nodes() const;
                [[nodiscard]] const mee::common::node &get_current() const;
                [[nodiscard]] std::size_t get_index() const noexcept;                
        };
    }
}

#endif // _MEE_HPP
