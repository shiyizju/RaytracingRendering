//
//  BufferedLineReader.cpp
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/25/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#include "BufferedLineReader.h"

BufferedLineReader::BufferedLineReader(const char *fileName, int maxBufferSize) : _maxBufferSize(maxBufferSize) {
    
    _file = fopen(fileName, "rb");
    _buffer = new char[_maxBufferSize];
    
    _currentBufferPosition = 0;
    _bufferSize = 0;
}

size_t BufferedLineReader::getLine(char *line, int maxLineSize) {
    
    size_t lineSize = 0;
    while (lineSize < maxLineSize) {
        // Reach buffer end.
        if (_currentBufferPosition == _bufferSize) {
            if (feof(_file)) {
                break;
            }
            else {
                _bufferSize = fread(_buffer, sizeof(char), _maxBufferSize, _file);
                _currentBufferPosition = 0;
            }
        }
        
        if (_buffer[_currentBufferPosition] == '\n') {
            _currentBufferPosition++;
            break;
        }
        
        line[lineSize++] = _buffer[_currentBufferPosition++];
    }
    
    return lineSize;
}