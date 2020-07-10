/*
MIT License

Copyright(c) 2019 David Krikheli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
Modification history:

*/

#ifndef dkutil_parallel_processor_base
#define dkutil_parallel_processor_base

namespace dk {

    /* This class represents a processor object that can partition the task at
       hand and process each partition independently of others.
       A simple example of such a task is to increment elements of a two
       dimentional array. The array can be logically broken up into groups of
       rows where each group can be processed independently of the others.
    */
    class ParallelProcessorBase
    {
    public:
        virtual ~ParallelProcessorBase();

        // This function is called by the driver to obtain information about
        // the size of the task at hand expressed in symbolic units. Based on
        // the size value the driver plans out how to partition the task and
        // then invokes the () operator once per partition as many times as
        // neccessary to execute the entire task.
        virtual size_t size() = 0;

        // This function is called by the driver to obtain information about
        // the maximum size of a partition expressed in symbolic units.
        // The driver object uses this information in a combination with the
        // the value returned by the size() function to plan out how to
        // partition the task at hand.
        // By default this function returns the value of 0 meaning partitions
        // can be of any size up until and including the size() value.
        // Derived classes may override this function to indicate that
        // partitions cannot exceed a particular value.
        virtual size_t partition_size_limit();

        // This operator is responsible for executing a partition of the task
        // at hand where the inxBegin and inxEnd parameters identify the exact
        // partition to execute.
        // Parameters:
        //  inxPartStart - inclusive index identifying the start of the partition.
        //  inxPartEnd   - exclusive index identifying the end of the partition.
        virtual void operator()(size_t inxPartStart, size_t inxPartEnd) = 0;
    };

} // namespace dk {

#endif // dkutil_parallel_processor_base
