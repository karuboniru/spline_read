#pragma once

#include <string>
#include <TF1.h>
#include <unordered_map>
#include <vector>

class splineread
{
private:
    std::unordered_map<std::string, TF1 *> splinelist;
    std::string version, genie_tune_name;
    bool uselog;
    std::vector<std::string> namelist;

public:
    // xmlfilename is the name to spline XML file.
    splineread(std::string xmlfilename);

    // copy initializer, will copy underlying data
    splineread(const splineread &rhs);

    // move initializer, will move ownership of underlying data
    splineread(splineread &&rhs);

    ~splineread();

    // get TF1 of a certain process by name tag
    TF1 *get_spline(std::string spline_name) const;

    // get a list of name tags
    std::vector<std::string> get_spline_list() const { return namelist; }

    // get version string in spline file
    std::string get_spline_version() const { return version; }

    // get uselog properties (is E sampled in log scale?)
    bool islog() const { return uselog; }

    // get tunetag like G00_00b_00_000
    std::string tunename() const { return genie_tune_name; }
};
