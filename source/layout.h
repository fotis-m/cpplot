/*
 * layout.h Brief overview sentence
 * 
 * Author:              Tom Clark  (thclark @ github)
 *
 * Copyright (c) 2019 Octue Ltd. All Rights Reserved.
 *
 */

#ifndef CPPLOT_LAYOUT_H
#define CPPLOT_LAYOUT_H

#include <exception>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <Eigen/Dense>
#include <json/single_include/nlohmann/json.hpp>


using nlohmann::json;


namespace cpplot {


/** @brief Enum of the three possible axis directions, X, Y, Z.
 */
enum AxisDirection { X, Y, Z};

/** @brief Axis class for use in layout of plots.
 *
 * Serialises to something like:
 * @code
 *     "xaxis": {"title": "x1"}
 * @endcode
 */
class Axis {
public:
    // Allow the serialiser function to access protected members
    friend void to_json(nlohmann::json& j, const Axis& p);

    Axis(const AxisDirection dir, const std::string title="") : title(title) {
        switch(dir){
            case X:
                key = "xaxis";
                break;
            case Y:
                key = "yaxis";
                break;
            case Z:
                key = "zaxis";
                break;
        }
    };

protected:
    std::string title;
    std::string key;
};


/** @brief Serialise axis into a valid json string.
 *
 * Produces json like:
 * @code
 *     "xaxis": {"title": "x1"}
 * @endcode
 */
void to_json(nlohmann::json& j, const Axis& p) {

//    nlohmann::json axis_title;
//    axis_title["text"] = p.title;
    nlohmann::json axis;
    axis["title"] = p.title;
    j[p.key] = axis;
}


class Layout {
public:

    // Allow the serialiser function to access protected members
    friend void to_json(nlohmann::json& j, const Layout& p);

    /** @brief Construct with default basic data (for test plots)
     *
     * Example use:
     * @code
     * Layout my_layout = Layout("a graph title"); // Default constructor Layout() also works
     * my_layout.xLabel("ecks");
     * my_layout.yLabel("why";
     *
     */
    Layout(const std::string title="") : title(title) {};

    void xLabel(const std::string label) {
        AxisDirection dir = X;
        Axis ax(dir, label);
        axes.push_back(ax);
    }

    void yLabel(const std::string label) {
        AxisDirection dir = Y;
        Axis ax(dir, label);
        axes.push_back(ax);
    }

    void zLabel(const std::string label) {
        AxisDirection dir = Z;
        Axis ax(dir, label);
        axes.push_back(ax);
    }

protected:

    std::vector<Axis> axes;
    std::string title;
};


/** @brief Serialise layout into a valid json string.
 *
 * Produces figure data JSON similar to:
 *      {"x": ["giraffes", "orangutans", "monkeys"],
 *       "y": [20.1, 14.4, 23.3],
 *       "type": "bar"}
 */
void to_json(nlohmann::json& j, const Layout& p) {

    if (!p.title.empty()) {
        j["title"] = p.title;
    };

    for (auto it = 0; it < p.axes.size(); it++) {
        nlohmann::json ax;
        to_json(ax, p.axes[it]);
        j.update(ax);
    };
}


}; // end namespace cpplot

#endif // CPPLOT_LAYOUT_H