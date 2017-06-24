/*
 * MIT License
 *
 * Copyright (c) 2017 EDDR Software, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Changes:
 * 2017-01-01: First & Last Name: What you did.
 * 2017-05-28: Kevin Nesmith: Initial contribution.
 *
 */

#ifndef OAFILEPARSER_H_
#define OAFILEPARSER_H_

#include <cstdio>

namespace oafp
{
    // Make sure the structs are kept to a 4 byte alignment.
    // If need be, add fields at the end to fullfill this requirement.
    struct fileHeader {
        unsigned int        testBit;        // 4 bytes
        unsigned short      type;           // 2 bytes - short 2
        unsigned short      schema;         // 2 bytes - back to even
        unsigned long       offset;         // 8 bytes
        unsigned int        size;           // 4 bytes
        unsigned int        used;           // 4 bytes
    };

    struct tableIndex {
        unsigned int        size;           // 4 bytes
        unsigned int        used;           // 4 bytes
        unsigned int        deleted;        // 4 bytes
        unsigned int        first;          // 4 bytes
    };

    struct appInfo {
        unsigned short appDataModelRev;     // 2 bytes - short 2
        unsigned short kitDataModelRev;     // 2 bytes - back to even
        unsigned short appAPIMinorRev;      // 2 bytes - short 2
        unsigned short kitReleaseNum;       // 2 bytes - back to even
    };

    class oaFileParser
    {
    public:
        int parse(const char *filePath);

    protected:
        virtual void onParsedPreface(unsigned int testBit, unsigned short type,
                                     unsigned short schema, unsigned long offset,
                                     unsigned int size, unsigned int used) = 0;
        virtual void onParsedTableInformation(unsigned long ids[],
                                              unsigned long offsets[],
                                              unsigned long sizes[], unsigned int num) = 0;
        virtual void onParsedFlags(unsigned int flags) = 0;
        virtual void onParsedTimeStamp(unsigned int timeStamp) = 0;
        virtual void onParsedLastSavedTime(unsigned long lastSavedTime) = 0;
        virtual void onParsedDatabaseMap(unsigned long ids[], unsigned int types[],
                                         unsigned int idCount, unsigned long tblIds[],
                                         unsigned int tblTypes[], unsigned int tblCount) = 0;
        virtual void onParsedStringTable(tableIndex table, const char *buffer) = 0;
        virtual void onParsedCreateTime(unsigned long createTime) = 0;
        virtual void onParsedDMandBuildName(unsigned short dataModelRev,
                                            const char *buildName) = 0;
        virtual void onParsedBuildInformation(unsigned short appDataModelRev,
                                              unsigned short kitDataModelRev,
                                              unsigned short appAPIMinorRev,
                                              unsigned short kitReleaseNum,
                                              const char *appBuildName,
                                              const char *kitBuildName,
                                              const char *platforName) = 0;
        virtual void onParsedDatabaseMapD(unsigned long ids[], unsigned int types[],
                                          unsigned long num) = 0;
        virtual void onParsedDatabaseMarker(unsigned int bitCheck) = 0;
        virtual void onParsedError(const char *error) = 0;

    private:
        void read0x04(FILE *file, unsigned long pos, unsigned long tblSize);
        void read0x05(FILE *file, unsigned long pos, unsigned long tblSize);
        void read0x06(FILE *file, unsigned long pos, unsigned long tblSize);
        void read0x07(FILE *file, unsigned long pos, unsigned long tblSize);
        void read0x0a(FILE *file, unsigned long pos, unsigned long tblSize);
        void read0x0b(FILE *file, unsigned long pos, unsigned long tblSize);
        void read0x19(FILE *file, unsigned long pos, unsigned long tblSize);
        void read0x1c(FILE *file, unsigned long pos, unsigned long tblSize);
        void read0x1d(FILE *file, unsigned long pos, unsigned long tblSize);
        void read0x1f(FILE *file, unsigned long pos, unsigned long tblSize);
        void read0x28(FILE *file, unsigned long pos, unsigned long tblSize);

    };

} // End namespace oafp

#endif //OAFILEPARSER_H_
