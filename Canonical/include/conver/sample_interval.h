#pragma once

struct SampleInterval
{
public:
    SampleInterval(int trials) : trials(trials) {}

    virtual void start() = 0;
    virtual void peekNextInterval(double &val, int &samples) = 0;
    virtual bool getNextInterval(double &val, int &samples) = 0;

    int getTrials() const { return trials; }

protected:
    int trials;
};

struct PowerTwoSampleInterval : public SampleInterval
{
public:
    PowerTwoSampleInterval(int trials, int max_degree) : SampleInterval(trials), max_degree(max_degree) { start(); }

    virtual void start()
    {
        current_degree = 0;
    }

    virtual void peekNextInterval(double &val, int &samples)
    {
        // what was val supposed to be again?
        val = 0.0;
        samples = pow(2, current_degree);
    }

    virtual bool getNextInterval(double &val, int &samples)
    {
        // what was val supposed to be again?
        val = 0.0;
        samples = pow(2, current_degree);

        if (current_degree == max_degree)
            return false;

        current_degree++;

        return true;
    }

protected:
    int max_degree;
    int current_degree;
};