#include "wii.h"
 
 
using namespace std;
 
char filename[MAXPATHLEN];
int id = 0;
char files[1000][80]; //1000 files with 80 chars
char *file = "";
int fcount = 0; // To count files
int longer = 0;
bool numbers = 0;
int nbLines(0);
u32 startLine, numLines;
string directory = "/";
string tempName;
int startfile = 0;
vector<string> dirs;
bool found;


 
 
// RETURNS NUMBER OF LINES IN FILE filename
int howManyLines (char *filename)
{
    FILE *fp;
    int numLines;
    char *c, line[1000];    // ONLY ALLOW 1000 CHARS PER LINE
 
    numLines = 0;
    fp = fopen(filename, "r");
    clrscr();
    if (fp)
    {
        do
        {
            c = fgets(line, 1000, fp);
            if (c)
                numLines++;
		printf("\033[2;0f");
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
    int i;
    int j;
    FILE *fp;
    char **lines;
    char currLine [1000];
    char loading = 196;
 
    fp = fopen(filename, "r");
    if (fp)
    {
        // MALLOC THE MEMORY FOR THE TOTAL ARRAY (SIZE OF FILE)
        fseek(fp, 0, SEEK_END);
        lines = (char**)malloc(ftell(fp));
	float equalses;
	float percentage;
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
	    printf("\033[2;0f");
	    equalses = (i / (numLines - 1)) * 80;
	    percentage = (i / (numLines - 1)) * 100;
	    for( j = 0; j < equalses;++j){
	    cout << loading;
	    }
	    cout << "\n" << percentage << "%  ";
        }

 
        // CLOSE THE FILE AND RETURN THE ARRAY
                fclose (fp);
        printf ("fp CLOSED\n");
        return lines;
    }
    else
    {
        printf ("Error, can't open file");
        return NULL;
    }
}
 
// DISPLAYS numLines LINES OF ARRAY STARTING AT startLine
int displayLines(int startLine, int numLines, char **lines, bool numbers, int totalLines)
{
    int i;
    int j;
    int longer = 0;
    for (j = startLine - 1; j < numLines + startLine - 1 - longer; j++)
    {
        if (totalLines == j) goto jumptodisplay; //to stop crashes on small files
        if (numbers == 1) longer = longer + ((strlen(lines[j]) + 7) / 80);
        if (numbers == 0) longer = longer + (strlen(lines[j]) / 80);
    }
    jumptodisplay:
    clrscr();
    for (i = startLine - 1; i < numLines + startLine - 1 - longer; i++)
    {
        if (totalLines == i) goto stopdisplaying; //to stop crashes on small files
        if (numbers == 1) printf ("%04d - %s", i + 1, lines[i]);
        if (numbers == 0) printf ("%s", lines[i]);
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
    id = 0;
    startfile = 0;
    dir = diropen (location.c_str());
 
    if (dir == NULL) //If empty
    {
        cout << " Unable to open the directory.\n";
        cout << " Returning to root.\n";
        sleep(2);
	clrscr();
        cout << "\x1b[5;5H" << endl;
        directory = "/";
        goto before;
 
    }
    else
    {
        while (dirnext(dir,filename,&st) == 0)
        {
 
 
            if ((st.st_mode & S_IFDIR ? "DIR":"FILE")=="FILE")// st.st_mode & S_IFDIR indicates a directory
            {
                strcpy(files[fcount], filename);
                ++fcount;
            }
            else
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
                cout << "\x1b[5;5H" << endl;
                --id;
                if(id<startfile) --startfile;
                file = files[id];
                Select(id);
 
            }
 
 
        }
 
        if (WPAD_ButtonsUp(0) & WPAD_BUTTON_RIGHT)
        {
            if (id+1<fcount-5)
            {
 
		clrscr();
                cout << "\x1b[5;5H" << endl;
                id += 5;
                if(id>startfile+19) startfile=id-19;
                file = files[id];
                Select(id);
 
            }
            else
            {
		clrscr();
                cout << "\x1b[5;5H" << endl;
                id += (fcount-id)-1;
                if(id>startfile+19) startfile=id-19;
                file = files[id];
                Select(id);
 
 
            }
 
 
        }
 
        if (WPAD_ButtonsUp(0) & WPAD_BUTTON_LEFT)
        {
            if (id>=5)
            {
 
		clrscr();
                cout << "\x1b[5;5H" << endl;
                id -= 5;
                if(id<startfile) startfile=id;
                file = files[id];
                Select(id);
 
            }
            else
            {
		clrscr();
                cout << "\x1b[5;5H" << endl;
                id -= id;
                if(id<startfile) startfile=id;
                file = files[id];
                Select(id);
 
 
            }
 
 
        }
 
        if (WPAD_ButtonsUp(0) & WPAD_BUTTON_DOWN)
        {
 
            if (id+1<fcount)
            {
		clrscr();
                cout << "\x1b[5;5H" << endl;
                ++id;
                if(id>startfile+19) ++startfile;
                file = files[id];
                Select(id);
            }
 
 
        }
        if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_A)
        {
            //open file and load in memory
            found = false;
            if(file != "." || file != "..")
            {
                for(int n = 0; n < dirs.size();n++)
                {
                    if(file == dirs[n]) { found = true; break; }
 
                }
            }
 
            string namefile = file;
            if(namefile == ".")
            {
		clrscr();
                cout << "\x1b[5;5H" << endl;
                directory = "/";
                sleep(1);
                List(directory);
                Select(id);
                selectFiles();
            }
            else if(namefile == "..")
            {
                directory = directory.substr(0, directory.size()-1);
                directory = directory.substr(0, directory.rfind("/")+1);
                sleep(1);
		clrscr();
                cout << "\x1b[5;5H" << endl;
                List(directory);
                Select(id);
                selectFiles();
            }
            else if(!found)
            {
                file = files[id];
                cout << "\033[2J";
                cout << "\n\n";
            }
            else{
                usleep(150000);
		clrscr();
                cout << "\x1b[5;5H" << endl;
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
    bool bad = true;
    bool question;
    while (bad)
    {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if ( pressed & WPAD_BUTTON_HOME ) exit(0);
        if ( pressed & WPAD_BUTTON_1 )
        {
            bad = false;
            question = 1;
        }
        if ( pressed & WPAD_BUTTON_2 )
        {
            bad = false;
            question = 0;
        }
 
    }
    return question;
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
    bool numbers = 0;
    bool question = 0;



    /*cout << "Would you like line numbers to be displayed? (press 1 for yes, 2 for no):";
    showNumbers(numbers);*/
 
    char **lines;
 
    
	if (argc >= 2)
	{
		cout << "It has been detected that you have specified an argument. Do you want this to be loaded as your file? Press 1 for yes, 2 for no. The argument you specified is: " << argv[1];
                question = LoadArgumentAsFile();
		if (question == 1){tempName = argv[1]; goto startofstuff;}
	}
    first:    
    clrscr();
    cout << "\x1b[0;5H" << endl;
    boxDrawing();
 
 
    cout << "\x1b[5;5H" << endl;
    List(directory);
    Select(id);
    selectFiles();
 
    cout << "\x1b[37;1m";
 
 
    tempName = directory + file;
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
    printf ("GOT HERE\n");
 
    // IF NULL THEN FILE COULDN'T BE OPENED SO EXIT
    if (lines == NULL)
    {
        printf ("\nFile could not be opened (for some reason)");
        sleep(2);
        goto first;
    }
 
    // DISPLAY LINES 1-20
    startLine = 1;
     int longer = displayLines(startLine, 20, lines, numbers, numLines);
    while (1)
    {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        // USER PRESSED 'UP' > SCROLL UP
 
        if ( pressed & WPAD_BUTTON_UP )
        {
            if (startLine != 1)
            {
                startLine --;
                longer = displayLines (startLine, 20, lines, numbers, numLines);
            }
        }
 
        // USER PRESSED 'DOWN' > SCROLL DOWN
        if ( pressed & WPAD_BUTTON_DOWN )
        {
            if (startLine < numLines - 20 + 1 + longer & numLines > 20 - longer)
            {
                startLine ++;
                 longer = displayLines (startLine, 20, lines, numbers, numLines);
            }
 
        }
        if ( pressed & WPAD_BUTTON_1 )
        {
            goto first;
        }
        if ( pressed & WPAD_BUTTON_2 )
        {
	    directory = "/";
            goto veryfirst;
        }
         if ( pressed & WPAD_BUTTON_A )
        {
            if (numbers == 0) {numbers = 1; goto skip;}
            if (numbers == 1) numbers = 0;
            skip:
            longer = displayLines (startLine, 20, lines, numbers, numLines);
        }
        if ( pressed & WPAD_BUTTON_HOME )
        {
	    clrscr();
            cout << "\x1b[2;10H" << endl;
            cout << "\nReturning to loader..." << endl;
            exit(0);
        }
    }
    return 0;
}