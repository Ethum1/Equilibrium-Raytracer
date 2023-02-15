#include "Sampling.h"

void random(std::vector<Vector2f>& samples) {
	for (int i = 0; i < samples.size(); ++i) {
		samples[i].setX(EQMath::random(0.0f, 0.999f));
		samples[i].setY(EQMath::random(0.0f, 0.999f));
	}
}

void jitter(std::vector<Vector2f>& samples) {
	int numSamples = samples.size();

	if (!isPerfect(numSamples)) {
		numSamples = nearestPerfect(numSamples);
	}

	int sqrtSamples = static_cast<int>(std::sqrt(numSamples));

	for (int i = 0; i < sqrtSamples; ++i) {
		for (int j = 0; j < sqrtSamples; ++j) {
			float x = (static_cast<float>(i) + EQMath::random(0.0f, 0.999f)) / static_cast<float>(sqrtSamples);
			float y = (static_cast<float>(j) + EQMath::random(0.0f, 0.999f)) / static_cast<float>(sqrtSamples);
			samples[i * sqrtSamples + j].setX(x);
			samples[i * sqrtSamples + j].setY(y);
		}
	}
}

void multiJitter(std::vector<Vector2f>& samples) {
	int numSamples = samples.size();

	if (!isPerfect(numSamples)) {
		numSamples = nearestPerfect(numSamples);
	}

	int sqrtSamples = static_cast<int>(std::sqrt(numSamples));
	float subcellWidth = 1.0f / static_cast<float>(numSamples);

	for (int i = 0; i < sqrtSamples; ++i) {
		for (int j = 0; j < sqrtSamples; ++j) {
			samples[i * sqrtSamples + j].setX(i * sqrtSamples * subcellWidth + j * subcellWidth + EQMath::random(0.0f, 0.999f) * subcellWidth);
			samples[i * sqrtSamples + j].setY(j * sqrtSamples * subcellWidth + i * subcellWidth + EQMath::random(0.0f, 0.999f) * subcellWidth);
		}
	}

	for (int i = 0; i < sqrtSamples; ++i) {
		for (int j = 0; j < sqrtSamples; ++j) {
			int k = j + static_cast<int>(EQMath::random(0.0f, 0.999f) * (sqrtSamples - j - 1));
			float t = samples[i * sqrtSamples + j].x();
			samples[i * sqrtSamples + j].setX(samples[i * sqrtSamples + k].x());
			samples[i * sqrtSamples + k].setX(t);

			k = j + static_cast<int>(EQMath::random(0.0f, 0.999f) * (sqrtSamples - j - 1));
			t = samples[j * sqrtSamples + i].y();
			samples[j * sqrtSamples + i].setY(samples[k * sqrtSamples + i].y());
			samples[k * sqrtSamples + i].setY(t);
		}
	}
}

void nrooks(std::vector<Vector2f>& samples) {
	for (int i = 0; i < samples.size(); ++i) {
		samples[i].setX((static_cast<float>(i) + EQMath::random(0.0f, 0.999f)) / static_cast<float>(samples.size()));
		samples[i].setY((static_cast<float>(i) + EQMath::random(0.0f, 0.999f)) / static_cast<float>(samples.size()));
	}

	for (int i = samples.size() - 2; i >= 0; --i) {
		int target = static_cast<int>(EQMath::random(0.0f, 0.999f) * static_cast<float>(i));
		float temp = samples[i + 1].x();
		samples[i + 1].setX(samples[target].x());
		samples[target].setX(temp);
	}
}

void shuffle(std::vector<Vector2f>& samples) {
	for (int i = samples.size() - 2; i >= 0; --i) {
		int target = static_cast<int>(EQMath::random(0.0f, 0.999f) * static_cast<float>(i));
		Vector2f temp = samples[i + 1];
		samples[i + 1] = samples[target];
		samples[target] = temp;
	}
}

void boxFilter(std::vector<Vector2f>& samples) {
	for (int i = 0; i < samples.size(); ++i) {
		samples[i].setX(samples[i].x() - 0.5f);
		samples[i].setY(samples[i].y() - 0.5f);
	}
}

void tentFilter(std::vector<Vector2f>& samples) {
	for (int i = 0; i < samples.size(); ++i) {
		float x = samples[i].x();
		float y = samples[i].y();

		if (x < 0.5f) {
			samples[i].setX(static_cast<float>(std::sqrt(2.0f * static_cast<float>(x)) - 1.0f));
		}
		else {
			samples[i].setX(1.0f - static_cast<float>(std::sqrt(2.0f - 2.0f * static_cast<float>(x))));
		}

		if (y < 0.5f) {
			samples[i].setY(static_cast<float>(std::sqrt(2.0f * static_cast<float>(y)) - 1.0f));
		}
		else {
			samples[i].setY(1.0f - static_cast<float>(std::sqrt(2.0f - 2.0f * static_cast<float>(y))));
		}
	}
}