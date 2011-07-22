 XKeymacs - Keyboard Utility for Windows
========================================

About
-----
XKeymacs provides key bindings like Emacs for applications running
on Microsoft Windows. You can also configure bindings for each
application.

Install
-------
1. Install the following runtime libraries. The latter is necessary
   only on 64bit versions of Windows.
   - Microsoft Visual C++ 2010 Redistributable Package (x86)
     <http://tinyurl.com/vc2010x86>
   - Microsoft Visual C++ 2010 Redistributable Package (x64)
     <http://tinyurl.com/vc2010x64>
2. Get a zip file from the following site.
   <http://sourceforge.jp/projects/xkeymacs/releases/>
3. Extract a directory containing XKeymacs from the zip file.
4. Move the directory in an appropriate directory.
5. Run xkeymacs.exe in the directory.

Upgrade or Uninstall
--------------------
1. Disable the "Auto Run" option of XKeymacs.
2. Exit XKeymacs as mentioned below.
3. Logoff or reboot windows.

   If you want to upgrade XKeymacs, you can follow the install instruction.

4. Delete the directory containing XKeymacs.
5. Run regedit and delete the following values:

        HKEY_CURRENT_USER\Software\oishi\XKeymacs
        HKEY_CURRENT_USER\Software\oishi\XKeymacs2
        HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run\XKeymacs

Usage
-----
### Run and exit

Run xkeymacs.exe. It automatically run xkeymacs64.exe on 64bit
versions of Windows. The gold key icon appears in the notification
area of the taskbar when XKeymacs is running.

Right-click the gold (or silver) key icon, and select Exit from the
pop up menu.

### Properties

Right-click the gold key icon and select "Properties...", you can
set key assigns per application.

Properties dialog has three tabs: Basic, Advanced and List tab.
The same configuration is shown by a different format in each tab.
In Basic tab, you can configure whether or not each key combination
works like Emacs. In Advanced tab, you can configure keys
assignments for each command. In Lis tab, you can take a look at the
configuration.

### Import and export properties

Select "Import Properties...", you can import an exported file.
Select "Export Properties...", you can export the configuration under the
following registry key to a file. 
**These features do not currently work on Windows 7.**

    HKEY_CURRENT_USER\Software\Oishi\XKeymacs2

### Options

Select "Options...", you can set whether or not:
1. run XKeymacs when you logon Windows.
2. enable file completion on the Command Prompt.
3. display C-x-, Shift, Ctrl and Alt LED in the notification area.

### Keyboard layout

Select "Keyboard Layout...", you can modify the keyboard layout.
Drag & drop a key from the base keyboard to the remapped keyboard.
The dropped key turns red. You can cancel the remap by clicking the red key.
**This feature does not currently work on Windows 7.**

XKeymacs only uses the feature of remapping scan codes provided by Windows,
so the remapping is not canceled after you disable or uninstall XKeymacs.
If you want to cancel remmapping without XKeymacs, delete the following
value by regedit.

    HKEY_CURRENT_USER\Keyboard Layout\Scancode Map
    HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout\Scancode Map

Support
-------
Web site
<http://xkeymacs.sourceforge.jp/>

Mailing list
<http://lists.sourceforge.jp/mailman/listinfo/xkeymacs-users-en>

Copyright
---------
Oishi-san <oishi@cam.hi-ho.ne.jp> owns the copyright of the code
derived from XKeymacs 3.48.
The following developers have the copyrights of their contributions.

* Kazuhiro Fujieda <fujieda@users.sourceforge.jp>
* Tomohiro Kashiwada <kikairoya@gmail.com>
* Harold Bamford <hbamford@users.sourceforge.net>

License
-------
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License on the followwing URL for more details.
<http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>

Acknowledgments
----------------
We thank Noguchi-san for creating Keymacs of the origin of XKeymacs,
Timothy-san for helping to write documentations, Hamada-san for
writing classes to save any data in kill-ring, and XKeymacs mailing
list members for suggesting new functions, reporting many defects,
and creating patches.

We thank Oishi-san for creating XKeymacs and continuing to develop
it for long years.

---
Kazuhiro Fujieda <fujieda@users.sourceforge.jp>
