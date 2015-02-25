//
//  BufferedLineReader.h
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/25/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#ifndef __RayTracingRendering__BufferedLineReader__
#define __RayTracingRendering__BufferedLineReader__

#include <stdio.h>

class BufferedLineReader {
private:
    FILE *_file;
    char *_buffer;
    unsigned int _currentBufferPosition;
    size_t _bufferSize;
    const size_t _maxBufferSize;
    
public:
    
    BufferedLineReader(const char *fileName, int maxBufferSize = 1024);
    
    size_t getLine(char *line, int maxLineSize);
    
    bool eof() {
        return feof(_file) && (_currentBufferPosition == _bufferSize);
    }
    
    ~BufferedLineReader() {
        delete [] _buffer;
        fclose(_file);
    }
};

#endif /* defined(__RayTracingRendering__BufferedLineReader__) */
