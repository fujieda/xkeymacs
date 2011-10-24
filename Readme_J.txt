 XKeymacs - Windows用キーボードユーティリティ
=============================================

概要
----
XKeymacsはEmacsのようなキー割り当てを、Windows上のアプリケーションで
実現します。アプリケーションごとにキー割り当てを変更することもできます。

インストール
------------
1. 以下のランタイムライブラリをインストールします。後者は64bit版の
   Windowsの場合のみ必要です。
   - Microsoft Visual C++ 2010 再頒布可能パッケージ (x86)
     <http://tinyurl.com/vc2010x86ja>
   - Microsoft Visual C++ 2010 再頒布可能パッケージ (x64)
     <http://tinyurl.com/vc2010x64ja>
2. zipファイルを次のサイトから取得します。
   <http://sourceforge.jp/projects/xkeymacs/releases/>
3. zipファイルを展開してXKeymacsの入ったディレクトリを取り出します。
4. そのディレクトリを適当なディレクトリに移動します。
5. ディレクトリの中のxkeymacs.exeを起動します。

アップグレードとアンインストール
--------------------------------
1. "ログオン時に実行"オプションを外します。
2. XKeymacsを終了します(後述)。
3. Windowsをログオフするか再起動します。

   アップグレードの場合にはインストールの手順に従ってください。 

4. XKeymacsを格納しているディレクトリを削除します。
3. レジストリエディタで次の値を削除します。

        HKEY_CURRENT_USER\Software\oishi\XKeymacs
        HKEY_CURRENT_USER\Software\oishi\XKeymacs2
        HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run\XKeymacs

使用方法
--------
### 起動と終了

xkeymacs.exeを実行します。64bit版のWindowsではxkeymacs64.exeが自動的
に実行されます。XKeymacsが起動すると、タスクバーに金の鍵が表示されます。

終了するには、タスクバーの金(または銀)の鍵のアイコンを右クリックし、
開いたメニューから"終了(X)"を選択します。

### プロパティ

金の鍵のアイコンを右クリックして「プロパティ...」を選択すると、
アプリケーションごとにXKeymacsのキー割り当てを設定できます。特に設定
しなければ標準の設定が利用されます。ダイアログにはダイアログ用の設定
を利用できます。

プロパティダイアログには、Basic、Advanced、Listの3つのタブがあります。
これらは同じ設定を異なる形式で表示します。Basicタブではキーの組み合わせが
Emacsのように機能するかどうかを、チェックボックスにより簡単に設定できます。
Advancedタブではコマンドごとにキー割り当てを設定できます。Listタブでは
行った設定を一覧できます。

### 設定の取り込みと書き出し

「設定の取り込み...」を選択すると、「設定の書き出し」によって書き出された
ファイルを読み込めます。「設定の書き出し...」を選択すると次のレジストリキー
以下のデータをファイルに書き出します。
**これらの機能は今のところWindows 7では機能しません。**

    HKEY_CURRENT_USER\Software\Oishi\XKeymacs2

### オプション

「オプション...」を選択すると、Windowsにログオンしたときに自動的に
XKeymacsを起動するか、コマンドプロンプトにおけるファイル名補完を有効に
するか、タスクバーのステータス領域にC-x, Meta, Shift, Ctrl, Altの
各状態を表示させるかを設定できます。

### キーボードレイアウト

「キーボードレイアウト...」を選択するとキーボードのレイアウトを変更
できます。上のベースキーボードのキーを下のリマップキーボードに
ドラッグ＆ドロップすると、下のリマップされたキーが赤くなります。
変更を取り消すときは、リマップキーボードの赤いキーをクリックします。
**この機能は今のところWindows 7では機能しません。**

XKeymacsはWindowsのスキャンコードの再割り当て機能を利用するだけなので、
XKeymacsを無効またはアンインストールしても設定は解除されません。
XKeymacsを用いずに設定を解除するには、レジストリエディタで以下の値を
消します。
HKEY_CURRENT_USER\Keyboard Layout\Scancode Map
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layout\Scancode Map

### その他

「無効にする」と「有効にする」でXKeymacsの無効/有効を切り替えられます。
C-qで有効/無効を切り替えることもできます。「リセット」はなにかのはずみで
XKeymacsが効かなくなったときに使います。

サポート
--------
ウェブサイト
<http://xkeymacs.sourceforge.jp/>

メーリングリスト
<http://lists.sourceforge.jp/mailman/listinfo/xkeymacs-users-ja>

著作権
------
XKeymacs 3.48に由来するコードの著作権は
大石さん<oishi@cam.hi-ho.ne.jp>
にあります。各自が貢献した部分の著作権を以下の開発者が持ちます。

* Kazuhiro Fujieda <fujieda@users.sourceforge.jp>
* Tomohiro Kashiwada <kikairoya@gmail.com>
* Harold Bamford <hbamford@users.sourceforge.net>
* co <cogood@gmail.com>

ライセンス
----------
このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフト
ウェア財団によって発行されたGNU一般公衆利用許諾契約書(バージョン2か、
希望によってはそれ以降のバージョンのうちどれか)の定める条件の下で
再頒布または改変することができます。

このプログラムは有用であることを願って頒布されますが、*全くの無保証*
です。商業可能性の保証や特定の目的への適合性は、言外に示されたものも
含め全く存在しません。詳しくは以下のURLのGNU一般公衆利用許諾契約書を
ご覧ください。
<hhttp://www.opensource.jp/gpl/gpl.ja.html>

謝辞
----
本プログラムの雛型となったKeymacsの作者である野口さん、英文ドキュメント
の作成のみならず、ドキュメントの構成等さまざまなアドバイスをしてくれた
Timothy-san、kill-ringにリッチテキストを始めとする任意のデータを保持
するためのクラスを作成してくれた浜田さん、XKeymacsに対し新機能の提案、
バグの報告、そしてパッチの作成をしていただいたXKeymacsメーリングリストの
皆様に感謝します。

XKeymacsを作成し、長年にわたって開発を続けてくださった大石さんに感謝
します。

---
Kazuhiro Fujieda <fujieda@users.sourceforge.jp>
