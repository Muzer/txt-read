#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/dir.h>
#include <keyboard_keysym.h>
#include <update.h>
#include <mxml.h>

#include "wii.h"


//Some things to keep the code tidy
#define clrscr()  cout << "\033[2J\033[2;0f"; // Clears screen and moves cursor to row 2, column 0
#define delay()   sleep(1); // waits for one second
#define moveIt()  cout << "\x1b[5;5H" << endl; // Moves cursor to row 5, column 0



//Some stuff for the wii's disc slot light
void _wiilight_turn(int enable);
static void * _wiilight_loop(void *arg);
static vu32 *_wiilight_reg = (u32*)0xCD0000C0;

void WIILIGHT_TurnOn();
int WIILIGHT_GetLevel();
int WIILIGHT_SetLevel(int level);

void WIILIGHT_Toggle();
void WIILIGHT_TurnOff();


using namespace std;

//Variables
char filename[MAXPATHLEN], files[1000][80];//1000 files with 80 chars, create a char array for storing the filenames
int id = 0, longer = 0, nbLines = 0, startfile = 0;//id = file selected, longer = the number of lines extra caused by lines being longer than 80 chars, nbLines = doesn't seem to be used, might delete this, startfile = id of file displayed at the top of the screen in the list of files, used for scrolling
int fcount = 0; // To count files
char *file = "";//Name of the selected file
char *numbers = "0";//numbers = whether line numbers are enabled (1) or not (0), found = whether the file is found (?)
char *autoupdate = "0";
bool found = false;
u32 startLine, numLines;// startLine = number of the line at the top of the screen, numLines = number of lines in file
string directory = "/", tempName;// directory sets the starting directory, tempName = directory + filename, or the first argument if one is supplied
vector<string> dirs;//The directories
int cursorline = 0;//Unused, ready for editting
int cursorpos = 0;//As Above
int menuselection = 0;//Selection in the main menu
int smenuselection = 0;//Selection in the settings menu

int boxDrawing();

void CreateXmlFile(char* filename)
{
   mxml_node_t *xml;
   mxml_node_t *data;
   mxml_node_t *group;

   xml = mxmlNewXML("1.0");

   data = mxmlNewElement(xml, "settings");
  
   //Create Some config value
   mxmlElementSetAttr(data, "numbers",numbers);
   
   mxmlElementSetAttr(data, "autoupdate",autoupdate);

   /* now lets save the xml file to a file! */
   FILE *fp;
   fp = fopen(filename, "w");

   mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);

   /*Time to clean up!*/
   fclose(fp);

}
void LoadXmlFile(char* filename)
{
   FILE *fp;
   mxml_node_t *tree;
   mxml_node_t *data;
   mxml_node_t *group;

   /*Load our xml file! */
   fp = fopen(filename, "r");
   tree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK);
   fclose(fp);

   /*Load and printf our values! */
   /* As a note, its a good idea to normally check if node* is NULL */
   data = mxmlFindElement(tree, tree, "settings", NULL, NULL, MXML_DESCEND);
   if(strcmp(mxmlElementGetAttr(data,"numbers"),"1") == 0){numbers = "1";}
   if(strcmp(mxmlElementGetAttr(data,"autoupdate"), "1") == 0){autoupdate = "1";}


   /* Yay Done! Now lets be considerate programmers, and put memory back how
      we found it before we started playing with it...*/
}


// RETURNS NUMBER OF LINES IN FILE filename. This is used to calculate the size that the array needs to be.
int howManyLines (char *filename)
{
	//Init variables and open the file, also init the wiilight and clear the screen.
    FILE *fp;
    int numLines = 0;
    char *c, line[1000];    // ONLY ALLOW 1000 CHARS PER LINE, this will not affect it when it is actually loaded into RAM.

    fp = fopen(filename, "r");

    clrscr();
    WIILIGHT_TurnOn();

    if (fp)
    {
        do
        {
        	//Get a line from the file, and store it into variable line. Also put it into c
            c = fgets(line, 1000, fp);

            //If this is a valid line
            if (c)
            	//add one to the number of lines, and do some snazzy effects including a spinning bar and fading of the light...
                numLines++;
            cout << "\033[2;0f";
            cout << "Counting number of lines.\nPlease wait. ";
            if (numLines % 80 < 20) cout << "|";
            if (numLines % 80 > 19 & numLines % 80 < 40) cout << "/";
            if (numLines % 80 > 39 & numLines % 80 < 60) cout << "-";
            if (numLines % 80 > 59) cout << "\\";
	    WIILIGHT_SetLevel(numLines % 256);
	    
        }
        while (c);
        //Close the file

        fclose (fp);

    }
    return numLines;
}

// RETURNS AN ARRAY OF STRINGS FROM FILE filename. This array is from then on used for everything.
char **createArrayFromFile(char *filename, float numLines)
{
	//init some variables, open the file and init the wiilight
    int i, j;
    FILE *fp;
    char **lines;
    char currLine [1000], loading = 196;

    fp = fopen(filename, "r"); // OPENING FILE
    WIILIGHT_TurnOn();
    if (fp)
    {
        // MALLOC THE MEMORY FOR THE TOTAL ARRAY (SIZE OF FILE)
        fseek(fp, 0, SEEK_END);
        lines = (char**)malloc(ftell(fp));
        float equalses, percentage;//Used for loading bar and percentage respectively
	int light;//Brightness of light
		//Go back to the beginning of the file and clear the screen
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

            // GO TO POSITION 2,0 AND CALCULATE PERCENTAGE
            printf("\033[2;0f");
            equalses = (i / (numLines - 1)) * 80;
            percentage = (i / (numLines - 1)) * 100;
	    light = (i / (numLines - 1)) * 255;
			//PRINT THE LOADING BAR, PERCENTAGE, AND FADE THE LIGHT
            for ( j = 0; j < equalses;++j)
            {
                cout << loading;
            }
            cout << "\n" << percentage << "%  ";
	    WIILIGHT_SetLevel(light);
	    
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
int displayLines(int startLine, int numLines, char **lines, string numbers, int totalLines)
{
	//Init variables
    int i, j, longer = 0;
    char vert_bar = 179;
    string display_str = "";
    
    for (j = startLine - 1; j < numLines + startLine - 1 - longer; j++)
    {
        if (totalLines == j) break; //if the number of lines in the file reaches j, STOP, otherwise it will crash on smaller files...

        if (numbers == "1") longer = longer + ((strlen(lines[j]) + 7) / 80); //If numbers are on, add the number of extra lines due to line longness the wii will display to longer, but add 7 to the line length (number of chars in line number)
        else longer = longer + (strlen(lines[j]) / 80);//Else, add the number of extra lines due to line longness the wii will display to longer (don't add 7)
    }


    clrscr();
    for (i = startLine - 1; i < numLines + startLine - 1 - longer; i++)
    {
        if (totalLines == i) break; //if the number of lines in the file reaches i, STOP, otherwise it will crash on smaller files...

        if (numbers == "1") printf ("%04d - %s", i + 1, lines[i]);//Printf the line number plus the line if numbers are on. Note: printf is used here because I don't know how to display numbers with a fixed number of digits using cout...
        else cout << lines[i];//Otherwise just cout the line
    }

    return longer;
}

//This gets called whenever a directory is opened, it initialises the directory and puts its contents into an array
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

//List directory
void Select(int iSelected)
{
    int x;

    boxDrawing();

    for (x=startfile;x!=startfile + 20;x++)
    {
        if (x==fcount) return; //this needed if things in current dir < 20

        if (x==iSelected)
        {
            cout << ">> " << "\x1b[47;1m\x1b[30m" << files[x]<< "\x1b[40;0m\x1b[37;1m" << endl;

        }
        else
        {
            cout <<  setw(3) << " "  << files[x] << endl;
        }
    }

}


//Controls for file selection...
int selectFiles()
{
    while (1)
    {
keyboardEvent nav;
        WPAD_ScanPads();
	KEYBOARD_ScanKeyboards();
	KEYBOARD_getEvent(&nav);
        if ((WPAD_ButtonsUp(0) & WPAD_BUTTON_UP) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_UP)))
        {
            if (id>=1)
            {
                clrscr();
                moveIt();
                --id;
                if (id<startfile) --startfile;
                file = files[id];
                Select(id);
		while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            }


        }

        if ((WPAD_ButtonsUp(0) & WPAD_BUTTON_RIGHT) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_RIGHT)))
        {
            if (id+1<fcount-5)
            {

                clrscr();
                moveIt();
                id += 5;
                if (id>startfile+19) startfile=id-19;
                file = files[id];
                Select(id);
		while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            }
            else
            {
                clrscr();
                moveIt();
                id += (fcount-id)-1;
                if (id>startfile+19) startfile=id-19;
                file = files[id];
                Select(id);
		while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}

            }


        }

        if ((WPAD_ButtonsUp(0) & WPAD_BUTTON_LEFT) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_LEFT)))
        {
            if (id>=5)
            {

                clrscr();
                moveIt();
                id -= 5;
                if (id<startfile) startfile=id;
                file = files[id];
                Select(id);
		while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            }
            else
            {
                clrscr();
                moveIt();
                id -= id;
                if (id<startfile) startfile=id;
                file = files[id];
                Select(id);
		while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            }

        }

        if ((WPAD_ButtonsUp(0) & WPAD_BUTTON_DOWN) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_DOWN)))
        {
            if (id+1<fcount)
            {
                clrscr();
                moveIt();
                ++id;
                if (id>startfile+19) ++startfile;
                file = files[id];
                Select(id);
		while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            }
        }

        if ((WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_RETURN)))
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
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            return 0;
        }
        if ( (WPAD_ButtonsHeld(0) & WPAD_BUTTON_HOME) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_F4)) )
        {
            clrscr();
            cout << "\x1b[2;10H" << endl;
            cout << "\nReturning to loader..." << endl;
            exit(0);
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
        }
    }

}

//Controls for whether to use the wiiload (or possibly Wii HL, but untested) argument as the file or not.
bool LoadArgumentAsFile()
{
    while (1)
    {
keyboardEvent nav;
        WPAD_ScanPads();
	KEYBOARD_ScanKeyboards();
	KEYBOARD_getEvent(&nav);
        if ( (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_F4)) ) exit(0);

        if ( (WPAD_ButtonsDown(0) & WPAD_BUTTON_1) || (nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_y))
        {
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            return true;
        }
        if ( (WPAD_ButtonsDown(0) & WPAD_BUTTON_2) || (nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_n) )
        {
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            return false;
        }

    }

}

//Broken code for USB Mass Storage, if anyone can get this working I would be happy...

// bool LoadFrom()
// {
// while (1)
//     {
// keyboardEvent nav;
//         WPAD_ScanPads();
// 	KEYBOARD_ScanKeyboards();
// 	KEYBOARD_getEvent(&nav);
//         if ( (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_F4)) ) exit(0);
// 
//         if ( (WPAD_ButtonsDown(0) & WPAD_BUTTON_1) || (nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_y))
//         {
// 	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
//             return true;
//         }
//         if ( (WPAD_ButtonsDown(0) & WPAD_BUTTON_2) || (nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_n) )
//         {
// 	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
//             return false;
//         }
// 
//     }
// }

//Prints the Main menu
void mainmenu(){
    clrscr();
    cout << "txt-read Main Menu\n\n";
    if(menuselection == 0) {cout << ">> " << "\x1b[47;1m\x1b[30m" << "Load a file from SD"<< "\x1b[40;0m\x1b[37;1m" << endl;}
    else{cout <<  setw(3) << " "  << "Load a file from SD" << endl;}
    if(menuselection == 1) {cout << ">> " << "\x1b[47;1m\x1b[30m" << "Update to the latest stable version of txt-read"<< "\x1b[40;0m\x1b[37;1m" << endl;}
    else{cout <<  setw(3) << " "  << "Update to the latest stable version of txt-read" << endl;}
    if(menuselection == 2) {cout << ">> " << "\x1b[47;1m\x1b[30m" << "Update to the latest unstable (svn) version of txt-read"<< "\x1b[40;0m\x1b[37;1m" << endl;}
    else{cout <<  setw(3) << " "  << "Update to the latest unstable (svn) version of txt-read" << endl;}
    if(menuselection == 3) {cout << ">> " << "\x1b[47;1m\x1b[30m" << "Settings"<< "\x1b[40;0m\x1b[37;1m" << endl;}
    else{cout <<  setw(3) << " "  << "Settings" << endl;}
    if(menuselection == 4) {cout << ">> " << "\x1b[47;1m\x1b[30m" << "Exit to loader"<< "\x1b[40;0m\x1b[37;1m" << endl;}
    else{cout <<  setw(3) << " "  << "Exit to loader" << endl;}
}

//Placeholder for network update, Come on AlexLH!
void networkupdate(string type){
clrscr();
FILE *f;
s32 main_server;
if(type == "svn"){
	WPAD_ScanPads();
	cout << "Please note: The meta.xml on the SVN version is most likely out of date, and there is no readme. Please wait for the official release to properly learn of the changes. If you find any bugs, please report them to the Google Code bugtracker, my blog or forum, muzerakascooby@gmail.com or the Wiibrew talk page. Any other sites I will not see. All SVN versions SHOULD have all of the previous functions working. Press A to continue, or B to quit to menu" << endl << endl << "*******END OF MESSAGES FROM TXT-READ. UNTIL THE NEXT NOTE LIKE THIS, ALL THE MESSAGES ARE FROM LIBWIIUPDATE, SO DISREGARD THEM*******" << endl << endl;
	sleep(2);
	WPAD_ScanPads();
	while (!(WPAD_ButtonsDown(0) & WPAD_BUTTON_A)){WPAD_ScanPads();if(WPAD_ButtonsDown(0) & WPAD_BUTTON_B){sleep(1);return;}}
	load_network();
	main_server = connect_to_server("74.86.133.219");
	f = fopen(get_location("boot.dol", "/apps/txt-read"), "wb+");
	send_message(main_server, "/svn/boot.dol", "www.muzer.wiibrew.exofire.net");
	instructions_update();
	get_file(main_server, f);
	fclose(f);
	net_close(main_server);
	load_network();
	main_server = connect_to_server("74.86.133.219");
	f = fopen(get_location("meta.xml", "/apps/txt-read"), "wb+");
	send_message(main_server, "/stable/meta.xml", "www.muzer.wiibrew.exofire.net");
	instructions_update();
	get_file(main_server, f);
	fclose(f);
	net_close(main_server);
	load_network();
	main_server = connect_to_server("74.86.133.219");
	f = fopen(get_location("icon.png", "/apps/txt-read"), "wb+");
	send_message(main_server, "/stable/icon.png", "www.muzer.wiibrew.exofire.net");
	instructions_update();
	get_file(main_server, f);
	fclose(f);
	net_close(main_server);
	cout << endl << endl << "*******MESSAGE FROM TXT-READ*******" << endl << endl << "Updating done. Please restart the app to see the changes take effect. Please note that there may or may not have been an update, it will download the latest version regardless. Press A..." << endl << endl << endl;
	WPAD_ScanPads();
	while (!(WPAD_ButtonsDown(0) & WPAD_BUTTON_A)){WPAD_ScanPads();}
}
if(type == "stable"){
	WPAD_ScanPads();
	cout << "You will shortly update this app to the latest stable version. If you find any bugs, please report them to the Google Code bugtracker, my blog or forum, muzerakascooby@gmail.com or the Wiibrew talk page. Any other sites I will not see. If you wish to have more features, at the possible loss of stability, please choose the SVN option (you can only do this when you get back to the main menu.) Press A..." << endl << endl << "*******END OF MESSAGES FROM TXT-READ. UNTIL THE NEXT NOTE LIKE THIS, ALL THE MESSAGES ARE FROM LIBWIIUPDATE, SO DISREGARD THEM*******" << endl << endl;
	sleep(2);
	WPAD_ScanPads();
	while (!(WPAD_ButtonsDown(0) & WPAD_BUTTON_A)){WPAD_ScanPads();if(WPAD_ButtonsDown(0) & WPAD_BUTTON_B){sleep(1);return;}}
	load_network();
	main_server = connect_to_server("74.86.133.219");
	f = fopen(get_location("boot.dol", "/apps/txt-read"), "wb+");
	send_message(main_server, "/stable/boot.dol", "www.muzer.wiibrew.exofire.net");
	instructions_update();
	get_file(main_server, f);
	fclose(f);
	net_close(main_server);
	load_network();
	main_server = connect_to_server("74.86.133.219");
	f = fopen(get_location("meta.xml", "/apps/txt-read"), "wb+");
	send_message(main_server, "/stable/meta.xml", "www.muzer.wiibrew.exofire.net");
	instructions_update();
	get_file(main_server, f);
	fclose(f);
	net_close(main_server);
	load_network();
	main_server = connect_to_server("74.86.133.219");
	f = fopen(get_location("icon.png", "/apps/txt-read"), "wb+");
	send_message(main_server, "/stable/icon.png", "www.muzer.wiibrew.exofire.net");
	instructions_update();
	get_file(main_server, f);
	fclose(f);
	net_close(main_server);
	cout << endl << endl << "*******MESSAGE FROM TXT-READ*******" << endl << endl << "Updating done. Please restart the app to see the changes take effect. Please note that there may or may not have been an update, it will download the latest version regardless. Press A..." << endl << endl;
	WPAD_ScanPads();
	while (!(WPAD_ButtonsDown(0) & WPAD_BUTTON_A)){WPAD_ScanPads();}
}
}

void settingsmenudisplay(){
    clrscr();
    cout << "txt-read Settings\n\n";
    if(smenuselection == 0) {cout << ">> " << "\x1b[47;1m\x1b[30m" << "Display line numbers"<< "\x1b[40;0m\x1b[37;1m";}
    else{cout <<  setw(3) << " "  << "Display line numbers";}
    if(numbers=="1") cout << "                                                  On" << endl;
    if(numbers=="0") cout << "                                                  Off" << endl;
    if(smenuselection == 1) {cout << ">> " << "\x1b[47;1m\x1b[30m" << "Automatic svn update on boot (requires reboot for update to be used)"<< "\x1b[40;0m\x1b[37;1m";}
    else{cout <<  setw(3) << " "  << "Automatic svn update on boot (requires reboot for update to be used)";}
    if(autoupdate=="1") cout << "  On" << endl;
    if(autoupdate=="0") cout << "  Off" << endl;
    if(smenuselection == 2) {cout << ">> " << "\x1b[47;1m\x1b[30m" << "Save"<< "\x1b[40;0m\x1b[37;1m" << endl;}
    else{cout <<  setw(3) << " "  << "Save" << endl;}
    if(smenuselection == 3) {cout << ">> " << "\x1b[47;1m\x1b[30m" << "Return to menu"<< "\x1b[40;0m\x1b[37;1m" << endl;}
    else{cout <<  setw(3) << " "  << "Return to menu" << endl;}
}
void settingsmenu(){
sleep(2);
settingsmenudisplay();
while(1){
keyboardEvent nav;
        WPAD_ScanPads();
	KEYBOARD_ScanKeyboards();
	KEYBOARD_getEvent(&nav);
        if ((WPAD_ButtonsUp(0) & WPAD_BUTTON_UP) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_UP)))
        {
            if (smenuselection>0)
            {
                --smenuselection;
		settingsmenudisplay();
		while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            }


        }
        if ((WPAD_ButtonsUp(0) & WPAD_BUTTON_DOWN) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_DOWN)))
        {
            if (smenuselection<3)
            {
                ++smenuselection;
		settingsmenudisplay();
		while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            }


        }
	if ( (WPAD_ButtonsHeld(0) & WPAD_BUTTON_HOME) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_F4)) )
        {
            clrscr();
            cout << "\x1b[2;10H" << endl;
            cout << "\nReturning to loader..." << endl;
            exit(0);
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
        }
	if ( (WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_RETURN)) )
        {
            clrscr();
            if(smenuselection==0) {if (numbers=="1"){numbers = "0";}else{numbers = "1";}settingsmenudisplay();sleep(1);}
            if(smenuselection==1) {if (autoupdate=="1"){autoupdate = "0";}else{autoupdate = "1";}settingsmenudisplay();sleep(1);}
	    if(smenuselection==2) {CreateXmlFile("/txt-read-settings.xml");clrscr();cout << "Settings saved. Wait 3 secs...";sleep(3);settingsmenudisplay();}
	    if(smenuselection==3) return;
            while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
        }
}
}
//Controls for the menu
void menucontrols(){
while(1){
keyboardEvent nav;
        WPAD_ScanPads();
	KEYBOARD_ScanKeyboards();
	KEYBOARD_getEvent(&nav);
        if ((WPAD_ButtonsUp(0) & WPAD_BUTTON_UP) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_UP)))
        {
            if (menuselection>0)
            {
                --menuselection;
		mainmenu();
		while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            }


        }
        if ((WPAD_ButtonsUp(0) & WPAD_BUTTON_DOWN) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_DOWN)))
        {
            if (menuselection<4)
            {
                ++menuselection;
		mainmenu();
		while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
            }


        }
	if ( (WPAD_ButtonsHeld(0) & WPAD_BUTTON_HOME) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_F4)) )
        {
            clrscr();
            cout << "\x1b[2;10H" << endl;
            cout << "\nReturning to loader..." << endl;
            exit(0);
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
        }
	if ( (WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) || ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_RETURN)) )
        {
            clrscr();
            if(menuselection==0) return;
            if(menuselection==1) {networkupdate("stable");sleep(1);mainmenu();}
	    if(menuselection==2) {networkupdate("svn");sleep(1);mainmenu();}
	    if(menuselection==3) {settingsmenu();sleep(1);mainmenu();}
	    if(menuselection==4) {clrscr(); cout << "Returning to loader..."; exit(0);}
            while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
        }
}
}


//Main loop
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
keyboardEvent nav;
clrscr();

int xmlfilelines = howManyLines("/txt-read-settings.xml");
clrscr();
if(xmlfilelines == 0){clrscr();cout << "Settings file does not exist, creating...";sleep(5);CreateXmlFile("/txt-read-settings.xml");cout << " Created";sleep(2);}
LoadXmlFile("/txt-read-settings.xml");
if(autoupdate=="1"){cout << "Automatic update triggered, updating...";sleep(2);networkupdate("svn");sleep(2);}
veryfirst:
clrscr();

	//Display the main menu, allow the user to select an option, and once Load a file from SD is selected, wait for 2 seconds...
    if (argc == 2 && argv[1] != "")
    {
        cout << "It has been detected that you have specified an argument." << endl << "Do you want this to be loaded as your file? Press 1 for yes, 2 for no." << endl << "The argument you specified is: " << argv[1];
        if (LoadArgumentAsFile())//If they said yes
        {
            tempName = argv[1];//Let the filename be the argument
            goto startofstuff;//And skip to the action
        }
    }
    mainmenu();
    menucontrols();
    sleep(2);
	//Clear the screen, and make the big array
    clrscr();
    char **lines;
	//IF the user specified an argument and that argument is not null (sendelf fix), ask them if they want to load the file they specified

//Broken code for loading from USB
/*
    clrscr();
    cout << "\x1b[0;5H" << endl;
    cout << "Press 1 for Load from Sd Card" << endl;
    cout << "Press 2 for Load from USB" << endl;
    if(!LoadFrom())
    {
	drive = "fat4:/";
    }*/

first:

	//Clear screen, move cursor and print the box drawing
    clrscr();
    cout << "\x1b[0;5H" << endl;
    boxDrawing();

	//Move the cursor, Set directory to root, list the directory and allow user to select a file
    moveIt();
    List(directory);
    Select(id);
    selectFiles();

	//Do something with the cursor (can't be bothered to look what)
    cout << "\x1b[37;1m";
	//Generate the full path of the file to be loaded
    tempName = directory+file;

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

    // IF NULL THEN FILE COULDN'T BE OPENED SO RESTART
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
	KEYBOARD_ScanKeyboards();
	KEYBOARD_getEvent(&nav);
        // USER PRESSED 'UP' > SCROLL UP
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_UP)
        {
            if (startLine != 1)
            {
                startLine --;
                longer = displayLines (startLine, 20, lines, numbers, numLines);
            }
        }
        if ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_UP))
        {
            if (startLine != 1)
            {
                startLine --;
                longer = displayLines (startLine, 20, lines, numbers, numLines);
            }
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
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
        if ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_DOWN))
        {
            if (startLine < numLines - 20 + 1 + longer & numLines > 20 - longer)
            {
                startLine ++;
                longer = displayLines (startLine, 20, lines, numbers, numLines);
            }
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
        }
        //User pressed 1 or F12, so return to file selection
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_1)
        {
            goto first;
        }
        if ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_F12))
        {
            goto first;
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
        }
        //User pressed 2 or Esc, so return to mainmenu
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_2)
        {
            directory = "/";
            goto veryfirst;
        }
        if ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_ESCAPE))
        {
            directory = "/";
            goto veryfirst;
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
        }
        //User pressed A or F11 (shortcut in KWrite), so toggle line numbers
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A)
        {
            if (numbers == "0")
            {
                numbers = "1";
                goto skipwm;
            }
            else numbers = "0";
	    skipwm:
            longer = displayLines (startLine, 20, lines, numbers, numLines);
        }
        if ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_F11))
        {
            if (numbers == "0")
            {
                numbers = "1";
                goto skipkb;
            }
            else numbers = "0";
	    skipkb:
            longer = displayLines (startLine, 20, lines, numbers, numLines);
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
        }
        //User pressed HOME or F4 (comes from Alt+F4), so return to loader
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)
        {
            clrscr();
            cout << "\x1b[2;10H" << endl;
            cout << "\nReturning to loader..." << endl;
            exit(0);
        }
        if ((nav.type == KEYBOARD_PRESSED) && (nav.keysym.sym == KEYBOARD_F4))
        {
            clrscr();
            cout << "\x1b[2;10H" << endl;
            cout << "\nReturning to loader..." << endl;
            exit(0);
	    while(nav.type == KEYBOARD_PRESSED){KEYBOARD_ScanKeyboards();KEYBOARD_getEvent(&nav);}
        }
    }
    //Something really weird happened, not even sure if it is possible to get here, but return something or people will start getting edgy...
    return 0;
}
