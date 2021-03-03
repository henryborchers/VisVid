//
// Created by Borchers, Henry Samuel on 2/26/21.
//

#include "generate.h"
#include "Visualizer.h"
extern "C" {
#include "shared/pgm.h"
}
int make_pgm(const std::string &source, const std::string &output_file){
    Visualizer visualizer;
    visualizer.load(source);
    visualizer.init_video();
    visualizer.process();
    visualizer.rasterize();
    visImage *img = visualizer.get_image();
    image_save_pgm(img, output_file.c_str());

    return 0;
}