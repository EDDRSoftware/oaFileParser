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

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>

#include "oaFileParser.h"

using namespace std;

class MyTestParser : public oafp::oaFileParser
{
protected:
    virtual void onParsedPreface(unsigned int testBit, unsigned short type, unsigned short schema, unsigned long offset, unsigned int size, unsigned int used){
        cout << "File Preface Information: " << endl;
        cout << "\ttestBit: 0x" << setfill('0') << setw(8) << hex << testBit << endl;
        cout << "\ttype:    0x" << setfill('0') << setw(8) << hex << type    << endl;
        cout << "\tschema:  0x" << setfill('0') << setw(8) << hex << schema  << endl;
        cout << "\toffset:  0x" << setfill('0') << setw(8) << hex << offset  << endl;
        cout << "\tsize:    " << dec << size    << endl;
        cout << "\tused:    " << dec << used    << endl;
    };
    virtual void onParsedTableInformation(unsigned long ids[], unsigned long offsets[], unsigned long sizes[], unsigned int num){
        cout << "Table Information: " << endl;
        for(int i=0; i<num; ++i){
            cout << "\ttable #" << setw(3) << i << " type: 0x" << setfill('0') << setw(4) << hex << ids[i] << " offset: 0x" << setfill('0') << setw(16) << (hex) << offsets[i] << " tableSize: " << (dec) << sizes[i] << endl;
        }
    };
    virtual void onParsedFlags(unsigned int flags){
        cout << "Database Flags: 0x" << setw(8) << hex << flags << endl;
    };
    virtual void onParsedTimeStamp(unsigned int timeStamp){
        cout << "Database Timestamp: " << timeStamp << endl;
    };
    virtual void onParsedLastSavedTime(unsigned long lsTime){
        cout << "Database Last Saved Time: " << endl;
        struct tm *timeInfo;
        timeInfo = gmtime((time_t*)&lsTime);
        cout << "\tTime Raw:   " << lsTime << endl;
        cout << "\tGMT:        " << asctime(timeInfo);  // asctime puts the EOL for you.
        timeInfo = localtime((time_t*)&lsTime);
        cout << "\tLocal Time: " << asctime(timeInfo);
    };
    virtual void onParsedDatabaseMap(unsigned long ids[], unsigned int types[], unsigned int idCount, unsigned long tblIds[], unsigned int tblTypes[], unsigned int tblCount){
        cout << "Database Map" << endl;
        cout << "\tNumber of ids: " << idCount << endl;
        for(int i=0; i<idCount; ++i){
            cout << "\t\tids: 0x" << setfill('0') << setw(8) << hex << ids[i] << "\ttypes: 0x" << setfill('0') << setw(8) << hex << types[i] << endl;
        } 

        cout << "\tNumber of tables: " << tblCount << endl;
        for(int i=0; i<tblCount; ++i){
            cout << "\t\ttable ids: 0x" << setfill('0') << setw(8) << hex << tblIds[i] << "\ttable types: 0x" << setfill('0') << setw(8) << hex << tblTypes[i] << endl;
        } 
    };
    virtual void onParsedStringTable(unsigned int size, unsigned int used, unsigned int deleted, unsigned int first, const char *buffer){
        cout << "Database String Table: " << endl;
        cout << "\tSize:    " << size    << endl;
        cout << "\tUsed:    " << used    << endl;
        cout << "\tDeleted: " << deleted << endl;
        cout << "\tFirst:   " << first   << endl;
        unsigned int b = 0;
        unsigned int l = 0;
        cout << "\tStrings: ";
        while(b<used){
            const char *ptr = &buffer[b];
            l = strlen(ptr);
            cout << ptr << "|";
            b += l + 1;
        }
        cout << endl;
    };
    virtual void onParsedCreateTime(unsigned long createTime){
        cout << "Database Create Time: " << endl;
        struct tm *timeInfo;
        timeInfo = gmtime((time_t*)&createTime);
        cout << "\tCreate Time Raw:   0x" << createTime << endl;        
        cout << "\tCreate GMT:        "   << asctime(timeInfo);  // asctime puts the EOL for you.
        timeInfo = localtime((time_t*)&createTime);
        cout << "\tCreate Local Time: "   << asctime(timeInfo);
    };
    virtual void onParsedDMandBuildName(unsigned short dataModelRev, const char *buildName){
        cout << "Database Data Model Revision: " << endl;
        cout << "\tData Model Revision: " << dataModelRev << endl;        
        cout << "\tBuild Name:          " << buildName << endl;
    };
    virtual void onParsedBuildInformation(unsigned short appDataModelRev, unsigned short kitDataModelRev, unsigned short appAPIMinorRev, unsigned short kitReleaseNum, const char *appBuildName, const char *kitBuildName, const char *platforName){
        cout << "Database Software Build Information: " << endl;
        cout << "\tAppDataModelRev: " << appDataModelRev << endl;        
        cout << "\tKitDataModelRev: " << kitDataModelRev << endl;        
        cout << "\tAppAPIMinorRev:  " << appAPIMinorRev  << endl;        
        cout << "\tKitReleaseNum:   " << kitReleaseNum   << endl;        
        cout << "\tAppBuildName:    " << appBuildName    << endl;        
        cout << "\tKitBuildName:    " << kitBuildName    << endl;        
        cout << "\tPlatforName:     " << platforName     << endl;        
    };
    virtual void onParsedDatabaseMapD(unsigned long ids[], unsigned int types[], unsigned long num){
        cout << "Database Data Table Map Delta" << endl;
        cout << "\tNumber of New Data Tables: " << num << endl;
        for(int i=0; i<num; ++i){
            cout << "\t\tIDs: 0x" << setfill('0') << setw(8) << hex << ids[i] << "\tTypes: 0x" << setfill('0') << setw(8) << hex << setfill('0') << setw(8) << hex << types[i] << endl;
        }; 
    };
    virtual void onParsedDatabaseMarker(unsigned int bitCheck){
        cout << "Database EOD Marker: " << endl;
        cout << "\tMarker: " << bitCheck << endl;        
    };
    virtual void onParsedError(const char *error){
        cerr << error << endl;
    };
};


// ****************************************************************************
// main()
//
// This is the top level function that tests the parser.
// ****************************************************************************
int main(int argc, char *argv[])
{
    if(argc<2){
        cerr << "Missing .oa file as the only parameter." << endl;
        cerr << "Usage: ./testParser /path/to/file.oa" << endl;
        return 1;
    }

    MyTestParser parser;
    return parser.parse(argv[1]);
}

/*
        

            unsigned long tblIds[fh.used];
            unsigned long tblOffsets[fh.used];
            unsigned long tblSizes[fh.used];
            unsigned long startOffset = 0;
            for(int i=0; i<fh.used; ++i){
                in = fread(&tblIds[i], sizeof(tblIds[fh.used]), 1, file);
            }
            for(int i=0; i<fh.used; ++i){
                in = fread(&tblOffsets[i], sizeof(tblOffsets[fh.used]), 1, file);
            }
            for(int i=0; i<fh.used; ++i){
                in = fread(&tblSizes[i], sizeof(tblSizes[fh.used]), 1, file);
            }


            for(int i=0; i<fh.used; ++i){
                cout << "SWITCH: TableID: 0x" << (hex) << tblIds[i] << " startOffset: 0x" << (hex) << startOffset << " tblOffsets: 0x" << (hex) << tblOffsets[i] << endl;
                switch((int)tblIds[i]){
                    //Index Items; Offset start from startOffset
                    case 0x04: read0x04(file, startOffset + tblOffsets[i], tblSizes[i]);    break; //Flags??
                    case 0x05: read0x05(file, startOffset + tblOffsets[i], tblSizes[i]);    break; //Time stamp??
                    case 0x06: read0x06(file, startOffset + tblOffsets[i], tblSizes[i]);    break; //Last saved time
                    case 0x07: read0x07(file, startOffset + tblOffsets[i], tblSizes[i]);    break; //File mapping
                    case 0x19: read0x19(file, startOffset + tblOffsets[i], tblSizes[i]);    break; //Creation time
                    case 0x1c: read0x1c(file, startOffset + tblOffsets[i], tblSizes[i]);    break; //DM information and version numbers
                    case 0x1d: read0x1d(file, startOffset + tblOffsets[i], tblSizes[i]);    break; //Software build information
                    case 0x28: read0x28(file, startOffset + tblOffsets[i], tblSizes[i]);    break; //End of database marker??

                               //Non-Index Items; Offset start from 0
                    case 0x0a: read0x0a(file, tblOffsets[i], tblSizes[i]);                  break; //String table
                    default:
                               break;
                }
            }
            fclose(file);
        } catch (...) {
            cout << "ERROR: parsing file" << endl;
            return 1;
        }
        return 0;
    }

*/
