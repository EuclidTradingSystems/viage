#ifndef WRAPPED_CALCULATOR_HPP
#define WRAPPED_CALCULATOR_HPP

#include <duckx.hpp>

#include <base_wrapper.hpp>
#include <list.hpp>
#include <Item/senior_citizen.hpp>
#include "life_expectency.hpp"

namespace Data
{
class rent;
}

namespace Calculator
{
using namespace Wrapper;

class wrapped_calculator : public base_wrapper<list<senior_citizen>>
{
public:
    wrapped_calculator();

private:
    life_expectency exp;
    rent* rent;
    QLocale::Language lingo;
    std::string docxName;
    std::string docxPath;

    const std::string sex_string(const senior_citizen::sexes& sex);
    void calculate_rent();
    void write_to_file();

    void end_runs(duckx::Run& run);
    void skip_paragraphs(duckx::Paragraph& paragraphs, int n_skiped);
    void skip_runs(duckx::Run& runs, int n_skiped);
    void print_runs(duckx::Run& runs);
//    void print_duckx();
};
}

#endif // WRAPPED_CALCULATOR_HPP
