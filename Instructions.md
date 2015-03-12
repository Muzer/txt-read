
```
 ______   __   __   ______            ____     ____        __       __
|      |  \ \ / /  |      |          |  _ |   | ___|      /  \     |  \
|__  __|   \ X /   |__  __|   _____  | |_||   | |__      / /\ \    |   \
  |  |      X X      |  |    |_____| |____|   | ___|    /  __  \   | |> |
  |  |     / X \     |  |            | |\ \   | |__    /  /  \  \  |   /
  |__|    /_/ \_\    |__|            |_| \_\  |____|  /__/    \__\ |__/    v1.1
```

muzerakascooby@gmail.com

Muzer @ EFnet, Blitzed, Gamesurge, DeviantART, and QuakeNet IRC.

http://wiibrew.org/wiki/User:Muzer



README

txt-read is a simple text reader app for the Wii. It can read any file stored in plain text format. It uses the Wii's console, and has all the features you would expect from an app nowadays (Wiimote, Front SD, and Homebrew Channel support). "Me" in this document refers to Muzer, who wrote the readme, the other devs are referred to by name.



CREATORS HISTORY

txt-read was originally created by Muzer
Then many parts were rewritten by mattgentl
Then many features were added by Muzer
Then it was released
Then CraZzy added a file picker, and CraZzy and Muzer and mattgentl are currently working together.



CREDITS

Coders:
> Muzer
> mattgentl
> CraZzy

Lib makers:
> AlexLH (libwiiupdate)
> Davyg (libwiikeyboard)
> Bool (wiilight example)
> Beardface (libmxml port)
> svpe (libfat port)
> All the people who made Wii Homebrew possible!

Etc.
> GizmoTheG (icon.png)
> marcan (various things)
> Bobbings (a real life friend), for all his support and kind words of advice, and the donation of 20p.
> AlexLH (file space)
> WarpedFla (file space)



HOW TO INSTALL THE APP IN YOUR LOADER

There are many ways to run this app. Once you've run the app, go to the bit marked MAIN MENU.

Homebrew Channel - SD

Go into the Homebrew Channel folder in this package, and if you don't already have one, copy the apps folder to the SD card. If you do have an apps folder, just copy the txt-read folder into the apps folder.

Homebrew Channel - Wiiload

Go into the root directory of this archive, and type wiiload txt-read.dol. Remember, putting a file as the argument is still supported (ie typing wiiload txt-read.dol file.txt (where file.txt is on SD, you can't actual send files from a remote PC)).

Homebrew Channel - sendelf

Open sendelf, and choose the txt-read.dol file included in this archive. If you want to easily load a specific file that you know the path of, type the path into the argument field (remember: The wii uses / not \ and there are no drive letters. You can type data.txt to load the data.txt file on the root of the SD, or you can type files/example.txt to load file example.txt in folder files.)

Wii Homebrew Launcher

Copy the contents of the Homebrew Launcher folder (not the folder itself) onto the root of your SD card. Load by selecting the Homebrew option from the menu. Alternatively, you can use the Homebrew Channel edition, depending on how you store all your other apps (there is no difference to the app itself). Please note that txt-read cannot be used with Wii Homebrew Launcher's emulator function, until hell\_hibou makes it use arguments standardly.

Twilight Hack

Copy txt-read.dol onto your SD card and rename it to boot.dol

Other loaders

txt-read should, in theory, work with any other loader you happen to throw at it, along with all the loader's functions, as long as it uses elf or dol files (and if you want to use the updater, it must accept files called boot.dol) (exception - you cannot use the Wii Homebrew Launcher's emulator function (but you can load it normally from there), but believe me, I tried hard to get it to work). If it doesn't work with a certain loader you happen to find, please contact me.



FIRST BOOT

When you first boot the application, it will create a file on the root of your SD called txt-read-settings.xml. This file stores your prefered settings. You do not need to do anything, it will work automatically (with messages to tell you what it is doing). If you download a new version of txt-read which requires an updated xml file, the app will tell you that you need to update it. Doing this will overwrite your settings, so in the future, this readme will tell you what you need to do to update it, if you want to do it manually in order to keep your settings. If you choose not to update, then the app is highly likely to crash.



MAIN MENU

Once you have booted the application, it will give you a list of menu options. Use up and down to select the option, and press A or enter to choose it. These are:
Load filename.txt - Only appears if you specify an argument using wiiload or sendelf (see above), this allows you to load the filename that you gave (note that filename.txt will be replaced with the name you specified). In the future there will be another one of these for a favourite file you can specify in Settings.
Load a file from SD - Allows you to choose a text file to load from the SD card.
Update to the latest stable version of txt-read - Allows you to download the latest release version of txt-read. This will always be stable and will hopefully never have major bugs, or features that don't work, but will not have all of the very newest features.
Update to the latest unstable (svn) version of txt-read - Allows you to download the latest version of txt-read available to the public. This will usually have more features than the stable version, but may have more major bugs. This and the above option are useful for installing if you used wiiload or sendelf (when it reminds you that you have the latest version, tell it to install anyway).
Settings - Takes you to the settings menu where you can adjust and save the default settings.
Credits - Have a look at all the wonderful people who helped make txt-read.
Exit to loader - Quits txt-read back to however you loaded it.

Note that in almost all places in txt-read, you can press Home or F4 to exit to loader.


LOAD FILENAME.TXT

This will simply load the file specified in the argument, and jump directly to the reader, bypassing the directory list.



LOAD A FILE FROM SD

This will will give you a list of files on your SD card. Find a file that is plain text (it does not matter what the extension is, or even if it has one at all), and press "A" to load it. Note: To go up one directory, choose ".."

Controls for the file picker:
Up and Down - move up or down one file
Left and right - move up or down five files
A or enter - Load file or enter directory (note that while the ".." directory is implemented properly, the "." directory actually returns to root.)
Home or F4 - Quit the app back to whatever you used to load it



THE TEXT READER ITSELF

Once you've picked your file, the program will do quite a few operations. The two that take any amount of time are counting the number of lines in the file, and loading the file into the Wii's memory. Therefore, there is a rotating line (the | / - \ thing) for the former operation, and a progress bar and percentage for the latter. You will most likely not see these for any normal sized file.

After it's loaded (which is usually super-fast for ordinary sized text files) you will see your file. There are a few things you can do here.

Controls for the file viewer:
Up and Down - move up or down one line
1 or F12 - Return to file select screen, staying in the directory (folder) you were in
2 or Escape - Return to the main menu
A or F11 - Toggle line numbers on or off
Home or F4 - Quit the app back to whatever you used to load it



UPDATE TO THE LATEST STABLE VERSION OF TXT-READ

Simply select this option and follow the prompts. If the directory you loaded the file from = / then it installs into /apps/txt-read/boot.elf (useful for installing from wiiload or sendelf), otherwise it installs into the directory you loaded it from, as boot.elf (it also installs the meta.xml and icon.png files in the same way). This will always be stable and will hopefully never have major bugs, or features that don't work, but will not have all of the very newest features.



UPDATE TO THE LATEST UNSTABLE (SVN) VERSION OF TXT-READ

See above. Allows you to download the latest version of txt-read available to the public. This will usually have more features than the stable version, but may have more major bugs. This and the above option are useful for installing if you used wiiload or sendelf (when it reminds you that you have the latest version, tell it to install anyway).



SETTINGS

Selecting this option will bring you to a menu (with the same controls as the main menu, with A being used to toggle options) that allows you to set options.
Display line numbers - Press A with this option selected to toggle whether or not to display line numbers by default (this can be overrided by pressing A or F11 when the file is being displayed).
Automatic svn update on boot (requires reboot for update to be used) - Press A with this option selected to toggle whether or not to do an automatic SVN update (see above) when the application boots. It still asks you, though.
Save - Save all the options set on this screen so they will load on every boot. This saves them into a file called "txt-read-settings.xml".
Return to menu - Go back to the main menu.



CREDITS

Once you select this option, just wait and the credits will scroll by. It looks quite nice, considering that it is text-based, so take a look at it.



VERSION HISTORY

All versions starting with "0" are unstable betas.

Older versions are available on google code under deprecated versions.

0.1: First stable version

0.2: First released version, added code to stop lines longer than the Wii's display width from breaking it.

0.3: Added option to disable line numbers

0.4: Fixed bug where some lines will be missed off the end of files meeting specific requirements

0.5: Added file selector (currently buggy but works) (thanks to CraZzy).

1.0: Finished file selector (partially me, partially CraZzy) - with subdirectories and proper scrolling and skip 5 files feature, added quick usage info, fixed many bugs, added option to toggle numbers, added a return to selector feature, added a progress bar, reinstated support for arguments.

1.1: Proper main menu, credits, fixed some bugs, added saving of certain settings, libwiiupdate, USB Keyboard Support, Wii Disc Slot Lighting, and many more.