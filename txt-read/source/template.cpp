#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/dir.h>

#include "wii.h"


#define clrscr()  cout << "\033[2J\033[2;0f";
#define delay()   sleep(1);
#define moveIt()  cout << "\x1b[5;5H" << endl;




using namespace std;

char filename[MAXPATHLEN], files[1000][80];//1000 files with 80 chars
int id = 0, longer = 0, nbLines = 0, startfile = 0;
int fcount = 0; // To count files
char *file = "";
bool numbers = false, found;
u32 startLine, numLines;
string directory = "/", tempName;
vector<string> dirs;

int boxDrawing();


// RETURNS NUMBER OF LINES IN FILE filename
int howManyLines (char *filename)
{
    FILE *fp;
    int numLines = 0;
    char *c, line[1000];    // ONLY ALLOW 1000 CHARS PER LINE

    fp = fopen(filename, "r");
    clrscr();
    if (fp)
    {
        do
        {
            c = fgets(line, 1000, fp);
            if (c)
                numLines++;
            cout << "\033[2;0f";
            cout << "Counting number of lines.\nPlease wait. ";
            if (numLines % 80 < 20) cout << "|";
            if (numLines % 80 > 19 & numLines % 80 < 40) cout << "/";
            if (numLines % 80 > 39 & numLines % 80 < 60) cout << "-";
            if (numLines % 80 > 59) cout << "\\";
        }
        while (c);
        fclose (fp);
    }
    return numLines;
}

// RETURNS AN ARRAY OF STRINGS FROM FILE filename
char **createArrayFromFile(char *filename, float numLines)
{
    int i, j;
    FILE *fp;
    char **lines;
    char currLine [1000], loading = 196;

    fp = fopen(filename, "r"); // OPENING FILE

    if (fp)
    {
        // MALLOC THE MEMORY FOR THE TOTAL ARRAY (SIZE OF FILE)
        fseek(fp, 0, SEEK_END);
        lines = (char**)malloc(ftell(fp));
        float equalses, percentage;
        fseek(fp, 0, SEEK_SET);
        clrscr();

        for (i = 0; i < numLines; i++)
        {
            // READ IN A LINE
            fgets (currLine, 1000, fp);


            // MALLOC MEMORY IN THE ARRAY FOR THIS LINE
            lines[i] = (char *)malloc(strlen(currLine));


            // COPY THE LINE INTO THE ARRAY
            strcpy (lines[i], currLine);

            // CALCULATE PERCENTAGE
            printf("\033[2;0f");
            equalses = (i / (numLines - 1)) * 80;
            percentage = (i / (numLines - 1)) * 100;

            for ( j = 0; j < equalses;++j)
            {
                cout << loading;
            }
            cout << "\n" << percentage << "%  ";
        }


        // CLOSE THE FILE AND RETURN THE ARRAY
        fclose (fp);
        return lines;
    }
    else
    {
        cout << "Error, can't open file";
        return NULL;
    }
}

// DISPLAYS numLines LINES OF ARRAY STARTING AT startLine
int displayLines(int startLine, int numLines, char **lines, bool numbers, int totalLines)
{
    int i, j, longer = 0;

    for (j = startLine - 1; j < numLines + startLine - 1 - longer; j++)
    {
        if (totalLines == j) goto jumptodisplay; //to stop crashes on small files

        if (numbers) longer = longer + ((strlen(lines[j]) + 7) / 80);
        else longer = longer + (strlen(lines[j]) / 80);
    }

jumptodisplay:

    clrscr();
    for (i = startLine - 1; i < numLines + startLine - 1 - longer; i++)
    {
        if (totalLines == i) goto stopdisplaying; //to stop crashes on small files

        if (numbers) cout << i + 1 << "-" << lines[i];
        else cout << lines[i];
    }

stopdisplaying:
    return longer;
}

int List(string &location)
{
    fcount= 0;
//##########################################################
//Show all files in directory
//Open directory
    struct stat st;
    DIR_ITER* dir;

before:
    id = 0, startfile = 0;

    dir = diropen (location.c_str());

    if (dir == NULL) //If empty
    {
        cout << " Unable to open the directory.\nReturning to root.\n";
        sleep(2);
        clrscr();
        moveIt();
        directory = "/";
        goto before;

    }
    else
    {
        while (dirnext(dir,filename,&st) == 0)
        {


            if ((st.st_mode & S_IFDIR ? "DIR":"FILE")=="FILE")// IF IT'S FILE
            {
                strcpy(files[fcount], filename);
                ++fcount;
            }
            else // IF IT'S DIR
            {
                dirs.push_back(filename);
                strcpy(files[fcount], filename);
                ++fcount;
            }
        }
    }

    return 0;

}

void Select(int iSelected)
{
    int x;

    boxDrawing();

    for (x=startfile;x!=startfile + 20;x++)
    {
        if (x==fcount) goto safety; //safety, I think it might be needed if things in current dir < 20

        if (x==iSelected)
        {
            cout << ">> " << "\x1b[47;1m\x1b[30m" << files[x]<< "\x1b[40;0m\x1b[37;1m" << endl;

        }
        else
        {
            cout <<  setw(3) << " "  << files[x] << endl;
        }
    }

safety:;

}



int selectFiles()
{
    while (1)
    {
        WPAD_ScanPads();
        if (WPAD_ButtonsUp(0) & WPAD_BUTTON_UP)
        {
            if (id>=1)
            {
                clrscr();
                moveIt();
                --id;
                if (id<startfile) --startfile;
                file = files[id];
                Select(id);

            }


        }

        if (WPAD_ButtonsUp(0) & WPAD_BUTTON_RIGHT)
        {
            if (id+1<fcount-5)
            {

                clrscr();
                moveIt();
                id += 5;
                if (id>startfile+19) startfile=id-19;
                file = files[id];
                Select(id);

            }
            else
            {
                clrscr();
                moveIt();
                id += (fcount-id)-1;
                if (id>startfile+19) startfile=id-19;
                file = files[id];
                Select(id);


            }


        }

        if (WPAD_ButtonsUp(0) & WPAD_BUTTON_LEFT)
        {
            if (id>=5)
            {

                clrscr();
                moveIt();
                id -= 5;
                if (id<startfile) startfile=id;
                file = files[id];
                Select(id);

            }
            else
            {
                clrscr();
                moveIt();
                id -= id;
                if (id<startfile) startfile=id;
                file = files[id];
                Select(id);
            }

        }

        if (WPAD_ButtonsUp(0) & WPAD_BUTTON_DOWN)
        {
            if (id+1<fcount)
            {
                clrscr();
                moveIt();
                ++id;
                if (id>startfile+19) ++startfile;
                file = files[id];
                Select(id);
            }
        }

        if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_A)
        {
            //open file and load in memory
            found = false;
            if (file != "." || file != "..")
            {
                for (int n = 0; n < dirs.size();n++)
                {
                    if (file == dirs[n])
                    {
                        found = true;
                        goto foundYet;
                    }
                }
            }
            foundYet:
            string namefile = file;
            if (namefile == ".")
            {
                clrscr();
                moveIt();
                directory = "/";
                delay();
                List(directory);
                Select(id);
                selectFiles();
            }
            else if (namefile == "..")
            {
                directory = directory.substr(0, directory.size()-1);
                directory = directory.substr(0, directory.rfind("/")+1);
                delay();
                clrscr();
                moveIt();
                List(directory);
                Select(id);
                selectFiles();
            }
            else if (!found)
            {
                file = files[id];
                clrscr();
                cout << "\n\n";
            }
            else
            {
                usleep(150000);
                clrscr();
                moveIt();
                char *slash = "/";
                strcat(file, slash);
                directory += file;
                dirs.resize(0);


                List(directory);
                Select(id);
                selectFiles();
            }
            return 0;
        }
        if ( WPAD_ButtonsHeld(0) & WPAD_BUTTON_HOME )
        {
            clrscr();
            cout << "\x1b[2;10H" << endl;
            cout << "\nReturning to loader..." << endl;
            exit(0);
        }
    }

}

bool LoadArgumentAsFile()
{
    while (1)
    {
        WPAD_ScanPads();
        if ( WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME ) exit(0);

        if ( WPAD_ButtonsDown(0) & WPAD_BUTTON_1)
        {
            return true;
        }
        if ( WPAD_ButtonsDown(0) & WPAD_BUTTON_2 )
        {
            return false;
        }

    }

}


//---------------------------------------------------------------------------------
int main(int argc, char **argv)
{
//---------------------------------------------------------------------------------
    // Init sets up the environment
    init();
    // The console understands VT terminal escape codes
    // This positions the cursor on row 2, column 0
    // we can use variables for this with format codes too
    // e.g. printf ("\x1b[%d;%dH", row, column )

veryfirst:
    clrscr();
    char **lines;

    if (argc >= 2 && argv[1] != "")
    {
        cout << "It has been detected that you have specified an argument." << endl << "Do you want this to be loaded as your file? Press 1 for yes, 2 for no." << endl << "The argument you specified is: " << argv[1];
        if (LoadArgumentAsFile())
        {
            tempName = argv[1];
            goto startofstuff;
        }
    }

first:
    clrscr();
    cout << "\x1b[0;5H" << endl;
    boxDrawing();


    moveIt();
    List(directory);
    Select(id);
    selectFiles();

    cout << "\x1b[37;1m";




startofstuff:

    // GET NUMBER OF LINES IN TEXT FILE
    numLines = howManyLines((char*)tempName.c_str());

    // IF numLines == 0 THEN EMPTY / NON EXISTENT FILE SO EXIT
    if (numLines == 0)
    {
        printf ("\nFile has zero lines");
        sleep(2);
        goto first;
    }

    // LOAD FILE INTO ARRAY
    lines = createArrayFromFile((char*)tempName.c_str(), numLines);

    // IF NULL THEN FILE COULDN'T BE OPENED SO EXIT
    if (lines == NULL)
    {
        cout << "\nFile could not be opened (for some reason)";
        sleep(2);
        goto first;
    }

    // DISPLAY LINES 1-20
    startLine = 1;
    int longer = displayLines(startLine, 20, lines, numbers, numLines);

    while (1)
    {
        WPAD_ScanPads();

        // USER PRESSED 'UP' > SCROLL UP
        if ( WPAD_ButtonsDown(0) & WPAD_BUTTON_UP )
        {
            if (startLine != 1)
            {
                startLine --;
                longer = displayLines (startLine, 20, lines, numbers, numLines);
            }
        }

        // USER PRESSED 'DOWN' > SCROLL DOWN
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_DOWN)
        {
            if (startLine < numLines - 20 + 1 + longer & numLines > 20 - longer)
            {
                startLine ++;
                longer = displayLines (startLine, 20, lines, numbers, numLines);
            }

        }
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_1 )
        {
            goto first;
        }
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_2 )
        {
            directory = "/";
            goto veryfirst;
        }
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A )
        {
            if (!numbers)
            {
                numbers = true;
                goto skip;
            }
            else numbers = false;
skip:
            longer = displayLines (startLine, 20, lines, numbers, numLines);
        }
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)
        {
            clrscr();
            cout << "\x1b[2;10H" << endl;
            cout << "\nReturning to loader..." << endl;
            exit(0);
        }
    }
    return 0;
}