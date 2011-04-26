= XKeymacs for 64bit版Windows

== これは何か

XKeymacs version 3.47を64bit版のWindowsでも動作するように改良したもの
です。Windows 7でコンソールアプリケーションを正しく識別できないバグと、
Internet Explorer 7/8でテキストエリアがダイアログ扱いになるバグも直し
てあります。32bit版のWindowsで普通に使うこともできます。

== 必要なもの

Visual C++ 2010再頒布可能パッケージ(x86)と
http://www.microsoft.com/downloads/details.aspx?FamilyID=a7b7a05e-6de6-4d3a-a423-37bf0912db84&displayLang=ja
64bit版のWindowsで使うなら、Visual C++ 2010再頒布可能パッケージ(x64)
が必要です。
http://www.microsoft.com/downloads/details.aspx?familyid=BD512D9E-43C8-4655-81BF-9350143D5867&displaylang=ja

== 使い方

64bit版のバイナリと32bit版のバイナリがありますが、起動するのは32bit版
のxkeymacs.exeだけです。xkeymacs.exeが必要に応じて64bit版の
xkeymacs64.exeを起動します。

== 制限

32bitアプリケーションと64bitアプリケーションで、kill-ringとキーボード
マクロを共有できません。

== 動作原理

32bit版が設定の管理や通知アイコンの表示などを行います。64bit版は
64bit版のフックのインストールと実行だけを行います。32bit版と64bit版は
名前付きパイプで通信します。

== 既知のバグ

* Windows 7でキーボードレイアウトを変更できません。
* Windows Vistaと7で設定の取り込みと書き出しができません。

== ソースコード

sourceforge.jpの作業部屋のGitリポジトリから取得できます。
http://sourceforge.jp/users/fujieda/pf/xkeymacs64/
Visual C++ 2010 ExpressでコンパイルするにはWindows Driver Kit (WDK)
7.1.0とWindows SDK 7.1が必要です。

== 著作権

XKeymacs version 3.47から改変されていない部分の著作権は、大石
(oishi@cam.hi-ho.ne.jp)さんにあります。改変した部分の著作権は藤枝和宏
(fujieda@users.sourceforge.jp)にあります。

== ライセンス

このプログラムはフリーソフトウェアです。あなたはこれを、フリーソフト
ウェア財団によって発行された GNU 一般公衆利用許諾契約書(バージョン2か、
希望によってはそれ以降のバージョンのうちどれか)の定める条件の下で再頒
布または改変することができます。

このプログラムは有用であることを願って頒布されますが、*全くの無保証*
です。商業可能性の保証や特定の目的への適合性は、言外に示されたものも
含め全く存在しません。詳しくはGNU 一般公衆利用許諾契約書をご覧くださ
い。
 
GNU 一般公衆利用許諾契約書(バージョン2)
http://www.opensource.jp/gpl/gpl.ja.txt
