#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <splineread.hh>
#include <TF1.h>
#include <TGraph.h>

splineread::splineread(std::string xmlfilename)
{
    boost::property_tree::ptree pt;
    boost::property_tree::xml_parser::read_xml(xmlfilename, pt);
    const auto genie_xsec_spline_list = pt.get_child("genie_xsec_spline_list");
    version = genie_xsec_spline_list.get<std::string>("<xmlattr>.version");
    uselog = genie_xsec_spline_list.get<bool>("<xmlattr>.uselog");
    const auto genie_tune = genie_xsec_spline_list.get_child("genie_tune");
    genie_tune_name = genie_tune.get<std::string>("<xmlattr>.name");
    for (const auto &spline : genie_tune)
    {
        if (spline.first == std::string("spline"))
        {
            const auto spline_name = spline.second.get<std::string>("<xmlattr>.name");
            namelist.push_back(spline_name);
            const auto spline_nknots = spline.second.get<double>("<xmlattr>.nknots");
            // next version of CERN ROOT will provide TGraph::AddPoint(), in that case
            // we could emit those std::vector containers.
            std::vector<double> Elist, xsec_list;
            Elist.reserve(spline_nknots);
            xsec_list.reserve(spline_nknots);
            for (const auto &knot : spline.second)
            {
                if (knot.first == std::string("knot"))
                {
                    Elist.push_back(knot.second.get<double>("E"));
                    xsec_list.push_back(knot.second.get<double>("xsec"));
                }
            }
            TGraph gh(spline_nknots, &Elist[0], &xsec_list[0]);
            // TGraph::Eval can sometimes used to do envaluing, while
            // TF1 can do more things, initialize a TF1 instance here
            // by lambda, TGraph instance is copied in. Life cycle is
            // managed by TF1. Hope TF1 won't introduce memory leaks.
            splinelist.insert({spline_name, new TF1(
                                                spline_name.c_str(),
                                                [gh](double *x, double *) {
                                                    return gh.Eval(x[0]);
                                                },
                                                0, 1000, 0)});
        }
    }
}

splineread::splineread(const splineread &rhs) : version(rhs.version),
                                          uselog(rhs.uselog),
                                          genie_tune_name(rhs.genie_tune_name),
                                          namelist(rhs.namelist)
{
    for (const auto &i : rhs.splinelist)
    {
        splinelist[i.first] = (TF1 *)i.second->Clone();
    }
}

splineread::splineread(splineread &&rhs) : version(rhs.version),
                                           uselog(rhs.uselog),
                                           genie_tune_name(rhs.genie_tune_name),
                                           namelist(rhs.namelist)
{
    splinelist = rhs.splinelist;
    rhs.splinelist.clear();
}

splineread::~splineread()
{
    for (const auto &i : splinelist)
    {
        delete i.second;
    }
}

TF1 *splineread::get_spline(const std::string splinename) const
{
    auto f = splinelist.find(splinename);
    if (f == splinelist.end())
    {
        return nullptr;
    }
    else
    {
        return f->second;
    }
}
