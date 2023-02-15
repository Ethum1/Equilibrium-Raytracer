#pragma once

#include <vector>
#include <cmath>

#include "Vector.h"
#include "Math.h"

// Sample Utility Functions:

// To achieve anti-aliasing:
// 1. "Sample" multiple different rays within a pixel, running the path tracing algorithm on each
// 2. "Filter" the multiple samples by applying a weighted average according to a distribution
// The average of the samples is the resulting pixel color
// What we are doing here is generating coordinates 

enum PIXSAMPLETYPE {RANDOM, JITTER, NROOKS, MULTIJITTER};

void random(std::vector<Vector2f>& samples);

// Jitter and Multi Jitter require samples.size() is a perfect number

void jitter(std::vector<Vector2f>& samples);

void multiJitter(std::vector<Vector2f>& samples);

void nrooks(std::vector<Vector2f>& samples);

void shuffle(std::vector<Vector2f>& samples);

void boxFilter(std::vector<Vector2f>& samples);

void tentFilter(std::vector<Vector2f>& samples);

inline bool isPerfect(int a) {
	int closestRoot = static_cast<int>(std::sqrt(a));
	return (a == (closestRoot * closestRoot));
}

inline int nearestPerfect(int a) {
	return static_cast<int>(std::pow(std::floor(std::sqrt(a)), 2.0f));
}