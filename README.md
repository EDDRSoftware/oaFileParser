# Preface
Given the OpenOffice/Microsoft Office competing formats of the past and the Google/Oracle Java fighting over source code, it doesn't make sense to compete on a file format.  As it stands to reason, learning a file format for interoperability purposes is not only legal in the United States, users prefer this, as it gives them, the owners of the data,  more access, and control of what is truly theirs.  Their DATA.  

Therefore, instead of creating a new file format for the Electronic Design Automation industry's design data, using a pre-existing format and creating a library to read and write the well-known design data is the only logical solution.  This library is going to do just that.

# Description
The oaFileParser verifies OpenAccess file types including \*.oa and tech.db files.  The parser mechanism exposes the design data with callback functionality.

# Limitations
This library is only to parse, at the lowest level, the information contained in the OpenAccess files.  It does not provide an API to store this data in any persistent manner.  This is where the programmer must add their implementation on top of this library to make the data useful.

# Get Involved
If you or your company would like to participate in this project, please email us at support@eddrs.com.  If you want to do simple code or documentation fixes but do not want to be a participating party, please take a look at these instructions on how to create a new pull request https://help.github.com/articles/creating-a-pull-request/.

If you would like to ensure that this library covers a particular file or coverage of data in a file, please email us a test case file at support@eddrs.com or check it into the testData directory by following these instructions: https://help.github.com/articles/creating-a-pull-request/.  Please do not check-in any data that you don't want shared with the world.  We do not take any responsibility for design data that is part of this repository.

# To Get The Test Data
Make sure you pull down this repository using the recursive flag.
```sh
git clone --recursive https://github.com/EDDRSoftware/oaFileParser
```
If you don't pull down the recursive test data, then you will need to supply your own.

You can also pull down example test files from http://www.princeton.edu/~nverma/cadenceSetup_5.10.41/gpdk090_v4.4/libs.oa22/gpdk090/.
