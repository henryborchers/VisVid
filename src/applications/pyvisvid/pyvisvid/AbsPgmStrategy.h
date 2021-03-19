//
// Created by henry on 3/7/21.
//

#pragma once

class AbsPgmStrategy {

public:
    virtual int make_pgm(const std::string &source, const std::string &output_file) const = 0;
};

