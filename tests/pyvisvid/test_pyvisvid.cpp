 //
// Created by Borchers, Henry Samuel on 2/26/21.
//

#include <catch.hpp>
#include <pyvisvid/Visualizer.h>
#include <pyvisvid/generate.h>
#include <pyvisvid/utils.h>
extern "C"{
    #include "shared/pgm.h"
}

 TEST_CASE("Test version information"){
    std::string version_info = get_version();
    REQUIRE(!version_info.empty());
}

TEST_CASE("ddd"){
    std::string source = "/home/henry/CLionProjects/VisVid/dummy.mp4";
    std::string output_file = "/home/henry/CLionProjects/VisVid/cmake-build-debug/dummy.pgm";

     std::shared_ptr<VideoFile> video = std::make_shared<VideoFile>(source);
     Processor processor(video);
     const std::shared_ptr<visImage> img = processor.process();
     image_save_pgm(img.get(), output_file.c_str());

//    make_pgm(source, output_file);
//     Visualizer visualizer;
//     visualizer.load(source);
//     visualizer.init_video();
//     visualizer.process();
//     visualizer.rasterize();
//     visImage *img = visualizer.get_image();
//     image_save_pgm(img, output_file.c_str());
}
 TEST_CASE("ddd1"){
     std::string source = "/home/henry/CLionProjects/VisVid/dummy.mp4";
     std::string output_file = "/home/henry/CLionProjects/VisVid/cmake-build-debug/dummy.pgm";

     std::shared_ptr<VideoFilePlayer> video = std::make_shared<VideoFilePlayer>(source);
     Processor2 processor(video);
     const std::shared_ptr<visImage> img = processor.process();
     image_save_pgm(img.get(), output_file.c_str());


//    make_pgm(source, output_file);
//     Visualizer visualizer;
//     visualizer.load(source);
//     visualizer.init_video();
//     visualizer.process();
//     visualizer.rasterize();
//     visImage *img = visualizer.get_image();
//     image_save_pgm(img, output_file.c_str());
 }
 TEST_CASE("ddd2") {
     std::string source = "/home/henry/CLionProjects/VisVid/dummy.mp4";
     std::string output_file = "/home/henry/CLionProjects/VisVid/cmake-build-debug/dummy.pgm";

     //     TODO Make this work \/
     //     Processor processor();
     //     Buffer buffer;
     //     for (auto const frame: video.iter()){
     //         auto result = processor.process(frame);
     //         buffer.pushBack(result);
     //     }
     //     image_save_pgm2(buffer, output_file.c_str());)
     //^^^^^^^^^^^^^^^^^^^^^^^^^
 }