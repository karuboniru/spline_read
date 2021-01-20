#include <splineread.hh>
#include <string>
#include <iostream>
#include <TCanvas.h>
#include <TF1.h>

int main(int argc, char const *argv[])
{
    const std::string filepath = "/home/yan/gxspl-ALTsmall_me.xml";
    splineread spline(filepath);
    std::cout << "spline tag is\t" << spline.tunename() << std::endl;
    // ========================================================
    // auto slist = spline.get_spline_list();
    // for (const auto &i : slist)
    // {
    //     std::cout << i << std::endl;
    // }
    // ========================================================
    // Choose one as an example here
    // plot it to an pdf file
    auto f = spline.get_spline("genie::ReinSehgalRESPXSec/NoPauliBlock/nu:14;tgt:1000000010;N:2112;proc:Weak[CC],RES;res:5;");
    assert(f);
    f->SetTitle("spline;E;xsec");
    auto c1 = new TCanvas;
    f->Draw();
    c1->Print("test.pdf");
    return 0;
}
