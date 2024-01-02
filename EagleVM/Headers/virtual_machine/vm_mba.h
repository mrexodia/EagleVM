#pragma once
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <unordered_map>

enum truth_operator
{
	op_none,
	op_xor,
	op_plus,
	op_minus,
	op_and,
	op_or,
	op_mul
};

enum variable_modifier
{
	mod_none,
	mod_not
};

enum variable_xy
{
	var_x,
	var_y
};

enum variable_type
{
	type_none,
	variable,
	constant,
	expression
};

class mba_variable 
{
public:
    variable_type type;
    variable_modifier modifier;

    mba_variable();
    mba_variable(variable_type type, variable_modifier modifier);

    virtual std::string print() const;
    virtual void expand(const std::unique_ptr<mba_variable>& x, const std::unique_ptr<mba_variable>& y) = 0;

    virtual std::unique_ptr<mba_variable> clone() = 0;
};

class mba_var_exp : public mba_variable
{
public:
    std::array<std::unique_ptr<mba_variable>, 2> vars;
    truth_operator operation;

    // mba_var_exp(mba_var&& v1, mba_var&& v2, truth_operator v_operation, variable_modifier modifier = mod_none);
    // mba_var_exp(const mba_variable& v1, const mba_variable& v2, truth_operator v_operation, variable_modifier modifier = mod_none);
    mba_var_exp();
    mba_var_exp(const mba_var_exp& other);
    mba_var_exp(std::unique_ptr<mba_variable> v1, std::unique_ptr<mba_variable> v2, truth_operator operation, variable_modifier modifier = mod_none);

    std::string print() const override;
    void expand(const std::unique_ptr<mba_variable>& x, const std::unique_ptr<mba_variable>& y) override;

    std::unique_ptr<mba_variable> clone() override;
    std::unique_ptr<mba_var_exp> clone_exp();
};

template <typename T>
class mba_var_const : public mba_variable 
{
public:
    T value;

    mba_var_const(T value, variable_modifier modifier = mod_none)
        : mba_variable(constant, modifier), value(value) {}

    std::string print() const override 
    {
        return std::to_string(value);
    }

    void expand(const std::unique_ptr<mba_variable>& x, const std::unique_ptr<mba_variable>& y) override
    {
        return;
    }

    std::unique_ptr<mba_variable> clone() override 
    {
        return std::make_unique<mba_var_const>(*this);
    }
};

class mba_var_xy : public mba_variable 
{
public:
    variable_xy variable_type;

    mba_var_xy(variable_xy var, variable_modifier modifier = mod_none);

    std::string print() const override;
    void expand(const std::unique_ptr<mba_variable>& x, const std::unique_ptr<mba_variable>& y) override;

    std::unique_ptr<mba_variable> clone() override;
};

class vm_mba
{
public:
	vm_mba();

	void create_tree(truth_operator none, uint32_t max_expansions);

private:
	std::unordered_map<truth_operator, mba_var_exp> mba_truth;
};