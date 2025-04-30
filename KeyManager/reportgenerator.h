#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include "iointerface.h"

class ReportGenerator
{
    public:
        enum OutputFormat
        {
            Device,
            Pdf
        };
        ReportGenerator(IOInterface *ioInterface = nullptr);
        void createHandoverProtocol(unsigned int id, ReportGenerator::OutputFormat outputFormat);
    private:
        IOInterface *mIOInterface;

};

#endif // REPORTGENERATOR_H
