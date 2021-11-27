#include <cmath>
#include <matplot/matplot.h>
#include "Options.h"
#include "AddressList.h"
#include "Route.h"
#include "Graph.h"
using namespace matplot;

Graph::Graph()
{
    f = figure(true);
    ax = f->add_subplot(1, 1, 1);
    origin = 0;
    vertexID = 0;
    depotID = 0;
}

void Graph::update(std::vector<std::pair<size_t, size_t>>& e, std::vector<double>& xs, std::vector<double>& ys,std::string c)
{
    
    edges.insert(edges.end(), e.begin(), e.end());
    x.insert(x.end(), xs.begin(), xs.end());
    y.insert(y.end(), ys.begin(), ys.end());

    for (auto _tmp : e)
    {
        cs.push_back(c);
    }

    //hold({ ax }, on);
    //g->edges(e);
    //hold(on);

    //g = ax->digraph(e);
    //g->x_data(xs);
    //g->y_data(ys);
    
    //matplot::plot({ 1.0,1.0 }, { 2.0,double(vertexID) });

}

std::vector<std::pair<size_t,size_t>> Graph::updateEdges(AddressList& l, bool tour)
{
    int i,n;
    std::vector<std::pair<size_t, size_t>> e;
    vertexID++;
    if(!tour)
    {
        i = 0;
        n = l.size();
    }
    else
    {
        i = 1;
        n = l.size() - 1;
        e.push_back({ depotID,vertexID });
    }
    
    for (i; i < n; i++)
    {
        e.push_back({ vertexID,vertexID + 1 });
        vertexID++;
    }
    if (tour)
    {
        e.push_back({ vertexID,depotID });
    }
    return e;
}
std::string Graph::disp(AddressList& a, std::string c)
{
    return c + ":" + a.display() + " L = " + std::to_string(a.length());
}

std::string Graph::disp(Route& a, std::string c)
{
    return c + ":" + a.display() + " L = " + std::to_string(a.length());
}

void Graph::plot(AddressList& a , std::string c, bool tour)
{
    auto e = updateEdges(a,tour);
    auto xs = a.x(origin);
    auto ys = a.y(origin);
    if(!tour)
    {
        display.push_back(disp(a,c));
    }
    update(e,xs,ys,c);
}

void Graph::plot(Route& a, std::string c)
{
    display.push_back(disp(a,c));
    Graph::plot(a,c,true);
}

void Graph::finalize()
{
    for (int i = 0; i < edges.size(); i++)
    {
        auto ed = edges[i];
        int i1 = ed.first;
        int i2 = ed.second;
        std::cout << "[(" << x[i1] << "," << y[i1] << ") (" << x[i2] << "," << y[i2] << ")]" << std::endl;
        auto a = ax->arrow(x[i1], y[i1], x[i2], y[i2]);
        a->color(cs[i]);
        if (cs[i] == "blue")
        {
            a->line_width(4);
        }
        else a->line_width(2);
    }
    ax->limits_mode_automatic();
    makeLegend();
}

//void Graph::wipe()
//{
//    edges.clear();
//    x.clear();
//    y.clear();
//    display.clear();
//    cla();
//    f = figure(true);
//    ax = f->add_subplot(1, 1, 1);
//    origin = 0;
//    vertexID = 0;
//    depotID = 0;
//}

void Graph::show(bool screen)
{
    finalize();
    if (screen)
        matplot::show();
}

void Graph::save(std::string filename)
{
    if (filename == "")
    {
        std::cout << "Type filename to save as (default: plot)" << std::endl;
        std::cin >> filename;
    }
    matplot::save("img/" + filename + ".jpg");
}

void Graph::makeLegend()
{
    double n = display.size();
    double width = legend_space * (max(x)-min(x));
    double height = legend_space * (max(y)-min(y));
    double xmin = min(x) + width;
    double ymin = min(y) + height;
    for (int i = 0; i < n; i++)
    {
        ax->text(xmin , height * i + ymin, display[i]);
    }
}
