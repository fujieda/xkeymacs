======================================================================
 XKeymacs - Keyboard Utility for Windows
======================================================================

--- Abstract ---
XKeymacs produces emacs key assignments for applications running on Windows.
You can turn key assignments on/off per application. XKeymacs also provides 
some BASH style command completion for the DOS command prompt window.


--- Install ---
1. Get xkeymacs???.zip from the next site or other suitable site.
(??? is version number.)
http://www.cam.hi-ho.ne.jp/oishi/
2. Unzip xkeymacs???.zip. -> xkeymacs??? directory is unzipped.
3. Run xkeymacs.exe in xkeymacs??? directory.


--- Uninstall ---
1. Exit XKeymacs.
2. Delete the xkeymacs??? directory.
3. Run regedit and delete the next values:
HKEY_CURRENT_USER\Software\oishi\XKeymacs2
HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run\XKeymacs


--- Usage ---
How to Run:
Run the xkeymacs.exe.
The gold key icon appears in the taskbar when XKeymacs is running.

How to Exit:
Right click the gold (or silver) key icon,
and select Exit from the pop up menu.

How to Set Properties:
Right click the gold key icon and select "Properties...",
you can set key assigns per an application.
Properties dialog has three tabs: Basic, Advanced and List tab.
Same data is displayed by different format in each tab.
In Basic tab, you can configure Emacs like setting.
In Advanced tab, you can assign any key to any command.
In Lis tab, you can take a look at configuration.

Select "Import Properties...", you can import an exported file.
Select "Export Properties...", you can export data under next key to a file.
HKEY_CURRENT_USER\Software\Oishi\XKeymacs2

Select "Options...", you can set:
1) run XKeymacs when you logon Windows or not,
2) automatic completion for the Command Prompt when you press tab or not,
3) display C-x-, Shift, Ctrl and Alt LED in the taskbar's status area or not.

Select "Keyboard Layout...", you can modify keyboard layout.
This is for Windows NT, 2000, XP, Server 2003 and Vista exclusive use.
Drag & drop a key on base keyboard, and the result is reflected
on remapped keyboard in the red.
Click the red key, and you will cancel the change.
XKeymacs does nothing rather than rewriting registry provided by Windows,
so the change is not canceled when you disable or uninstall XKeymacs.
You want to cancel changes without using XKeymacs,
delete the next value by regedit.
HKEY_CURRENT_USER\Keyboard Layout\Scancode Map
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout\Scancode Map


--- Support ---
Web site
http://www.cam.hi-ho.ne.jp/oishi/indexen.html

Mailing List
You can subscribe this mailing list from
http://lists.sourceforge.net/lists/listinfo/xkeymacs-support
Please let me know your impression, question, request, etc.
I announce version up information here too.


--- Copyright ---
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


--- Acknowledgements ---
I thank Noguchi-san for making Keymacs, which is template for XKeymacs.
I thank Timothy-san for helping writing documentations.
I thank Hamada-san for writing classes to save any data in kill-ring.
And I thank XKeymacs mailing list members for suggesting new functions,
reporting many defects, and creating patches.


- ---
oishi@cam.hi-ho.ne.jp
