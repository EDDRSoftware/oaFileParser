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
 * 2017-06-28: Kevin Nesmith: Initial contribution.
 *
 */

#include "oaFileParser.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>


namespace oafp
{
    using namespace std;

    // Make sure the structs are kept to a 4 byte alignment.
    // If need be, add fields at the end to fullfill this requirement.
    struct fileHeader{
        unsigned int        testBit;        // 4 bytes
        unsigned short      type;           // 2 bytes - short 2
        unsigned short      schema;         // 2 bytes - back to even
        unsigned long       offset;         // 8 bytes
        unsigned int        size;           // 4 bytes
        unsigned int        used;           // 4 bytes
    };

    struct tableIndex{
        unsigned int        size;           // 4 bytes
        unsigned int        used;           // 4 bytes
        unsigned int        deleted;        // 4 bytes
        unsigned int        first;          // 4 bytes
    };

    unsigned long roundAlign8Bit(unsigned long len){
        unsigned long rem = len%8;
        return len + (8 - rem);
    }

    void oaFileParser::read0x04(FILE *file, unsigned long pos, unsigned long tblSize){
        fseek(file, pos, SEEK_SET);
        unsigned int flags = 0;
        unsigned int in = 0;
        in = fread(&flags, sizeof(flags), 1, file);
        onParsedFlags(flags);
    }

    void oaFileParser::read0x05(FILE *file, unsigned long pos, unsigned long tblSize){
        fseek(file, pos, SEEK_SET);
        unsigned int timeStamp;
        unsigned int in = 0;
        in = fread(&timeStamp, sizeof(timeStamp), 1, file);
        onParsedTimeStamp(timeStamp);
    }

    void oaFileParser::read0x06(FILE *file, unsigned long pos, unsigned long tblSize){
        fseek(file, pos, SEEK_SET);
        unsigned long lsTime;
        unsigned int in = 0;
        in = fread(&lsTime, sizeof(lsTime), 1, file);
        onParsedLastSavedTime(lsTime);
    }

    void oaFileParser::read0x07(FILE *file, unsigned long pos, unsigned long tblSize){
        fseek(file, pos, SEEK_SET);
        unsigned int numRes;
        unsigned int numData;
        unsigned int in = 0;
        in = fread(&numRes, sizeof(numRes), 1, file);
        in = fread(&numData, sizeof(numData), 1, file);
        unsigned int numOther = numData - numRes;
        unsigned long ids[numRes];
        unsigned int types[numRes];
        unsigned long tblIds[numOther];
        unsigned int tblTypes[numOther];
        for(int i=0; i<numRes; ++i){
            in = fread(&ids[i], sizeof(ids[i]), 1, file);
        }
        for(int i=0; i<numRes; ++i){
            in = fread(&types[i], sizeof(types[i]), 1, file);
        }

        for(int i=0; i<numOther; ++i){
            in = fread(&tblIds[i], sizeof(tblIds[i]), 1, file);
        }
        for(int i=0; i<numOther; ++i){
            in = fread(&tblTypes[i], sizeof(tblTypes[i]), 1, file);
        }
        onParsedDatabaseMap(ids, types, numRes, tblIds, tblTypes, numOther);
    }

    void oaFileParser::read0x0a(FILE *file, unsigned long pos, unsigned long tblSize){
        fseek(file, pos, SEEK_SET);
        char buffer[tblSize];
        unsigned int size;
        unsigned int used;
        unsigned int deleted;
        unsigned int first;
        unsigned int empty;
        unsigned int in = 0;
        in = fread(&size, sizeof(size), 1, file);
        in = fread(&used, sizeof(used), 1, file);
        in = fread(&deleted, sizeof(deleted), 1, file);
        in = fread(&first, sizeof(first), 1, file);
        in = fread(&empty, sizeof(empty), 1, file);
        in = fread(&buffer, sizeof(buffer), 1, file);
        onParsedStringTable(size, used, deleted, first, buffer);
    }

    void oaFileParser::read0x19(FILE *file, unsigned long pos, unsigned long tblSize){
        fseek(file, pos, SEEK_SET);
        unsigned long createTime;
        unsigned int in = 0;
        in = fread(&createTime, sizeof(createTime), 1, file);
        onParsedCreateTime(createTime);
    }

    void oaFileParser::read0x1c(FILE *file, unsigned long pos, unsigned long tblSize){
        fseek(file, pos, SEEK_SET);
        unsigned short dataModelRev;
        char buildName[tblSize - sizeof(dataModelRev)];
        unsigned int in = 0;
        in = fread(&dataModelRev, sizeof(dataModelRev), 1, file);
        in = fread(&buildName, sizeof(buildName), 1, file);
        onParsedDMandBuildName(dataModelRev, buildName);
    }

    void oaFileParser::read0x1d(FILE *file, unsigned long pos, unsigned long tblSize){
        fseek(file, pos, SEEK_SET);
        unsigned short appDataModelRev;
        unsigned short kitDataModelRev;
        unsigned short appAPIMinorRev;
        unsigned short kitReleaseNum;
        char buffer[tblSize - (sizeof(unsigned short)*4)];
        char *appBuildName;
        char *kitBuildName;
        char *platforName;
        unsigned int in = 0;
        in = fread(&appDataModelRev, sizeof(appDataModelRev), 1, file);
        in = fread(&kitDataModelRev, sizeof(kitDataModelRev), 1, file);
        in = fread(&appAPIMinorRev, sizeof(appAPIMinorRev), 1, file);
        in = fread(&kitReleaseNum, sizeof(kitReleaseNum), 1, file);
        in = fread(&buffer, sizeof(buffer), 1, file);
        unsigned int b = 0;
        appBuildName = &buffer[b];
        b += roundAlign8Bit(strlen(appBuildName));
        kitBuildName = &buffer[b];
        b += roundAlign8Bit(strlen(kitBuildName));
        platforName = &buffer[b];
        onParsedBuildInformation(appDataModelRev, kitDataModelRev, appAPIMinorRev, kitReleaseNum, appBuildName, kitBuildName, platforName);
    }

    void oaFileParser::read0x1f(FILE *file, unsigned long pos, unsigned long tblSize){
        fseek(file, pos, SEEK_SET);
        unsigned long num;
        unsigned int in = 0;
        in = fread(&num, sizeof(num), 1, file);
        unsigned long ids[num];
        unsigned int types[num];
        for(int i=0; i<num; ++i){
            in = fread(&ids[i], sizeof(ids[i]), 1, file);
        }
        for(int i=0; i<num; ++i){
            in = fread(&types[i], sizeof(types[i]), 1, file);
        }
        onParsedDatabaseMapD(ids, types, num);
    }

    void oaFileParser::read0x28(FILE *file, unsigned long pos, unsigned long tblSize){
        fseek(file, pos, SEEK_SET);
        unsigned int bitCheck;
        unsigned int in = 0;
        in = fread(&bitCheck, sizeof(bitCheck), 1, file);
        onParsedDatabaseMarker(bitCheck);
    }

    int oaFileParser::parse(const char* filePath)
    {
        try {
            fileHeader fh;
            FILE *file = fopen(filePath, "r");
            if(file==NULL){
                throw("File path does not exist.");
                return 1;
            }
        
            size_t in = fread(&fh, sizeof(fh), 1, file);
            onParsedPreface(fh.testBit, fh.type, fh.schema, fh.offset, fh.size, fh.used);

            unsigned long ids[fh.used];
            unsigned long offsets[fh.used];
            unsigned long sizes[fh.used];
            unsigned long startOffset = 0;
            for(int i=0; i<fh.used; ++i){
                in = fread(&ids[i], sizeof(ids[fh.used]), 1, file);
            }
            for(int i=0; i<fh.used; ++i){
                in = fread(&offsets[i], sizeof(offsets[fh.used]), 1, file);
            }
            for(int i=0; i<fh.used; ++i){
                in = fread(&sizes[i], sizeof(sizes[fh.used]), 1, file);
            }

            onParsedTableInformation(ids, offsets, sizes, fh.used);
            for(int i=0; i<fh.used; ++i){
                if(ids[i]==1){
                    startOffset = offsets[i];
                    break;
                }
            }

            for(int i=0; i<fh.used; ++i){
                switch((int)ids[i]){
                    //Index Items; Offset start from startOffset
                    case 0x04: read0x04(file, startOffset + offsets[i], sizes[i]);    break; //Flags??
                    case 0x05: read0x05(file, startOffset + offsets[i], sizes[i]);    break; //Time stamp??
                    case 0x06: read0x06(file, startOffset + offsets[i], sizes[i]);    break; //Last saved time
                    case 0x07: read0x07(file, startOffset + offsets[i], sizes[i]);    break; //File mapping
                    case 0x19: read0x19(file, startOffset + offsets[i], sizes[i]);    break; //Creation time
                    case 0x1c: read0x1c(file, startOffset + offsets[i], sizes[i]);    break; //DM information and version numbers
                    case 0x1d: read0x1d(file, startOffset + offsets[i], sizes[i]);    break; //Software build information
                    case 0x28: read0x28(file, startOffset + offsets[i], sizes[i]);    break; //End of database marker??

                               //Non-Index Items; Offset start from 0
                    case 0x0a: read0x0a(file, offsets[i], sizes[i]);                  break; //String table
                    default:
                               break;
                }
            }
            fclose(file);
        } catch (...) {
            cerr << "ERROR: parsing file" << endl;
            return 1;
        }
        return 0;
    }
} //End namespace oafp
