======================================================================
 XKeymacs - Windows 用 キーボードユーティリティ
======================================================================

--- 概要 ---
XKeymacsはEmacsのようなキー割り当てを、Windows上のアプリケーションで
実現します。アプリケーションごとにキー割り当てを変更することもできます。

--- インストール ---
1. 以下のランタイムライブラリをインストールします。後者は64bit版の
   Windowsの場合のみ必要です。
- Microsoft Visual C++ 2010 再頒布可能パッケージ (x86)
  http://tinyurl.com/vc2010x86ja
- Microsoft Visual C++ 2010 再頒布可能パッケージ (x64)
  http://tinyurl.com/vc2010x64ja
2. xkeymacs???.zipを下記サイトから取得します。(???はバージョン番号)
   http://sourceforge.jp/projects/xkeymacs/releases/
3. xkeymacs???.zipを展開してディレクトリxkeymacs???を得ます。
4. xkeymacs???を適当なディレクトリに移動します。
5. ディレクトリxkeymacs???の中のxkeymacs.exeを起動します。

--- アンインストール ---
1. XKeymacsを終了します(後述)。
2. ディレクトリxkeymacs???を削除します。
3. レジストリエディタで次の値を削除します。
HKEY_CURRENT_USER\Software\oishi\XKeymacs
HKEY_CURRENT_USER\Software\oishi\XKeymacs2
HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run\XKeymacs

--- 使用方法 ---
起動方法:
xkeymacs.exeを実行します。64bit版のWindowsではxkeymacs64.exeが自動的
に実行されます。XKeymacsが起動すると、タスクバーに金の鍵が表示されます。

終了方法:
タスクバーの金(または銀)の鍵のアイコンを右クリックし、開いたメニュー
から"終了(X)"を選択します。

各種設定方法:
金の鍵のアイコンを右クリックするとXKeymacsのメニューにアクセスできます。

プロパティ...
アプリケーションごとにXKeymacsのキー割り当てを設定できる。特に設定し
なければ、標準の設定が利用されます。ダイアログには、ダイアログ用の設
定を利用できます。

プロパティダイアログには、Basic、Advanced、Listの3つのタブがあります。
これらは同じ設定を異なる形式で表示します。Basicタブではキーの組み合わ
せがEmacsのように機能するかどうかを、チェックボックスにより簡単に設定
できます。Advancedタブではコマンドごとにキー割り当てを設定できます。
Listタブでは行った設定を一覧できます。

設定の取り込み...
「設定の書き出し」によって書き出されたファイルを読み込みます。
設定の書き出し...
HKEY_CURRENT_USER\Software\Oishi\XKeymacs2以下のデータをファイルに書
き出します。
** これらの機能は今のところWindows 7では機能しません。 **

オプション...
Windowsにログオンしたときに自動的にXKeymacsを起動するか、コマンドプロ
ンプトにおけるファイル名補完を有効にするか、タスクバーのステータス領
域にC-x, Meta, Shift, Ctrl, Altの各状態を表示させるかを設定できます。

キーボードレイアウト...
キーボードのレイアウトを変更できます。上のベースキーボードのキーを下
のリマップキーボードにドラッグ＆ドロップすると、下のリマップされたキー
が赤くなります。変更を取り消すときは、リマップキーボードの赤いキー
をクリックします。
** この機能は今のところWindows 7では機能しません。 **

XKeymacsはWindowsのスキャンコードの再割り当て機能を利用するだけなので、
XKeymacsを無効またはアンインストールしても設定は解除されません。
XKeymacsを用いずに設定を解除するには、レジストリエディタで以下の値を
消します。
HKEY_CURRENT_USER\Keyboard Layout\Scancode Map
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout\Scancode Map

無効/有効にする
C-qで有効/無効を切り替えることもできます。

リセット
なにかのはずみでXKeymacsが効かなくなったときはリセットすると直ります。

終了
XKeymacsを終了します。

ヘルプ
Readme_J.txtを表示します。

バージョン情報
バージョン情報を表示します。

--- サポート ---
ウェブサイト
http://sourceforge.jp/projects/xkeymacs/
https://github.com/fujieda/xkeymacs

メーリングリスト
http://lists.sourceforge.jp/mailman/listinfo/xkeymacs-users-ja

--- 著作権 ---
XKeymacs 3.48に由来するコードの著作権は
大石さん<oishi@cam.hi-ho.ne.jp>
にあります。
各自が貢献した部分の著作権を以下の開発者が持ちます。
Kazuhiro Fujieda <fujieda@users.sourceforge.jp>
Tomohiro Kashiwada <kikairoya@gmail.com>
Harold Bamford <hbamford@users.sourceforge.net>

--- ライセンス ---
このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフト
ウェア財団によって発行されたGNU一般公衆利用許諾契約書(バージョン2か、
希望によってはそれ以降のバージョンのうちどれか)の定める条件の下で再頒
布または改変することができます。

このプログラムは有用であることを願って頒布されますが、*全くの無保証*
です。商業可能性の保証や特定の目的への適合性は、言外に示されたものも
含め全く存在しません。詳しくは以下のURLのGNU一般公衆利用許諾契約書を
ご覧ください。
hhttp://www.opensource.jp/gpl/gpl.ja.html

--- 謝辞 ---
本プログラムの雛型となったKeymacsの作者である野口さん、英文ドキュメン
トの作成のみならず、ドキュメントの構成等さまざまなアドバイスをしてく
れたTimothy-san、kill-ringにリッチテキストを始めとする任意のデータを
保持するためのクラスを作成してくれた浜田さん、XKeymacsに対し新機能の
提案、バグの報告、そしてパッチの作成をしていただいたXKeymacsメーリン
グリストの皆様に感謝します。

XKeymacsを作成し、長年にわたって開発を続けてくださった大石さんに感謝
します。

---
Kazuhiro Fujieda <fujieda@users.sourceforge.jp>
