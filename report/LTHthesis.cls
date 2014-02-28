\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{LTHthesis}%
  [2013/10/03 A class for theses at LTH, based on memoir]

\newif\ifno@biblatex
\DeclareOption{nobiblatex}{%
  \no@biblatextrue
}

\DeclareOption{paper}{%
  \PassOptionsToClass{article}{memoir}%
  \AtEndOfClass{\renewcommand\thesection{\arabic{section}}
    \setsecnumdepth{subsection}
    \renewcommand\sectiondot{.}
    \renewcommand\thesubsection{\thesection.\arabic{subsection}}
    \let\paper\@paper
  }
}%

\let\possiblyShow@trims\relax
\DeclareOption{showtrims}{%
  \def\possiblyShow@trims{\stockaiv}%
  \PassOptionsToClass{\CurrentOption}{memoir}%
}%

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{memoir}}%

\ProcessOptions
\LoadClass{memoir}
\RequirePackage{amsmath,amsfonts,amssymb}
\AtBeginDocument{\RequirePackage{graphicx}}


%%%%%%%%%%% Paper and typeblock size %%%%%%%%%%%%%%%%
\settrimmedsize{220mm}{155mm}{*}
\setstocksize{220mm}{155mm}
\possiblyShow@trims
\setlength{\trimtop}{30mm}
\setlength{\trimtop}{\stockheight}
\addtolength{\trimtop}{-\paperheight}
\setlength{\trimtop}{0.5\trimtop}
\setlength{\trimedge}{\stockwidth}
\addtolength{\trimedge}{-\paperwidth}
\setlength{\trimedge}{0.5\trimedge}
\trimFrame

\textheight=178mm\textwidth=117mm
\setlrmargins{*}{*}{1.0}
\setulmargins{14mm}{*}{*}
\headheight=12pt\headsep=12pt
\checkandfixthelayout

%%%%%%%%% Headers and footers %%%%%%%%%%%%%%%%%%%%
\makepagestyle{thesis}
\makeevenhead{thesis}{\normalfont\itshape\leftmark}{}{}
\makeoddhead{thesis}{}{}{\normalfont\itshape\rightmark}
\makeoddfoot{thesis}{}{}{\normalfont\thepage}
\makeevenfoot{thesis}{\normalfont\thepage}{}{}
\copypagestyle{chapter}{thesis}
\makeevenhead{chapter}{}{}{}
\makeoddhead{chapter}{}{}{}
\pagestyle{thesis}

\parskip=0pt plus 0.1pt\relax
%%%%%%%%%%%%%% Part style%%%%%%%%%%%%%%%%%%%%%%%%%%%
% We want the part page to always be recto, so some extra work is 
% necessary.
\let \origPart=\part
\renewcommand*{\part}{\@openrighttrue\origPart}
\let\origPartpageend=\partpageend
\renewcommand*{\partpageend}{{\origPartpageend\@openrightfalse}}
\renewcommand*{\cftpartname}{Part~}
\aliaspagestyle{part}{empty}


%%%%%%%%%%% Chapter style %%%%%%%%%%%%%%%%
\makechapterstyle{LTHthesis}{
  \setlength{\beforechapskip}{0pt}
  \setlength{\afterchapskip}{20mm plus 3mm minus 1mm}
  \renewcommand\printchaptername{}
  \renewcommand\afterchapternum{\par\nobreak\vskip\midchapskip}
  \renewcommand\chapnumfont{\fontsize{36}{42pt}\usefont{T1}{pbk}{l}{n}}
  \renewcommand\chaptitlefont{\fontsize{24}{30pt}\usefont{T1}{pbk}{l}{n}}
  \renewcommand\printchapternum{\hrule height0pt\relax
              \chapnumfont\leftline{\thechapter}}
  \renewcommand\printchapternonum{
    \setbox0=\hbox{\chapnumfont 3}\hrule height 0pt
    \leftline{\vrule height \ht0 width 0pt}\vskip\midchapskip}%
  \def\@chap@mark##1{\ifnum \c@secnumdepth >\m@ne
    \if@mainmatter \@chapapp \ \thechapter . \ \fi \fi ##1}
  \def\chaptermark##1{\markboth{\@chap@mark{##1}}{\@chap@mark{##1}}}
}
\chapterstyle{LTHthesis}
\openright % Will be reset by the \tableofcontents command.

%%%%%%%%%%%%%%%% section style %%%%%%%%%%%%%%%%%%%
\setbeforesecskip{-20pt plus -6 pt minus -3pt}
\setsecheadstyle{\sffamily\bfseries\Large\raggedright}
\setaftersecskip{9pt}
\def\sectionmark#1{\markright{\ifnum \c@secnumdepth > \z@
    \thesection \quad\fi #1}}


%%%%%%%%%%%%%% subsection style %%%%%%%%%%%%%%%%%%%%
\setbeforesubsecskip{-10pt plus -5pt minus -2pt}
\setsubsecheadstyle{\sffamily\bfseries\large\raggedright}
\setaftersubsecskip{3pt}

%%%%%%%%%%%%% subsubsection style %%%%%%%%%%%%%%%%%%%%%%%%
\setbeforesubsubsecskip{6pt plus 2pt minus 1pt}
\setaftersubsubsecskip{-1em}
\setsubsubsecheadstyle{\normalsize\bfseries\itshape}

%%%%%%%%%%%%%%%%%%% Style for all section numbering %%%%%%%%%55555
\setsecnumformat{\csname the#1\endcsname\csname #1dot\endcsname\quad}
\newcommand\sectiondot{}
\newcommand\subsectiondot{}
\newcommand\subsubsectiondot{}
\newcommand\captiondot{}


%%%%%%%%%%%%%%%%% Table of contents %%%%%%%%%%%%%%%%%%%
\let\origTOC=\tableofcontents
\renewcommand\tableofcontents{\clearforchapter\origTOC*\cleartorecto\openany}
\renewcommand\cftchapteraftersnum{.}
\setlength{\cftchapternumwidth}{20pt}
\setlength{\cftsectionindent}{\cftchapternumwidth}
%\renewcommand\cftchapterpresnum{\hfill}
\setlength{\cftbeforechapterskip}{\smallskipamount}
\renewcommand\cftsectionaftersnum{\sectiondot}
\renewcommand\@pnumwidth{2em}

%%%%%%%%%%%%%% Captions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\captiondelim{\captiondot\quad}
\captionnamefont{\small\bfseries}
\captiontitlefont{\small}
\captionstyle[\centering]{}
\AtBeginDocument{%
  \dimen0=\textwidth\advance\dimen0 -2\parindent
  \captionwidth{\dimen0}\changecaptionwidth
}
  
\setlength{\textfloatsep}{12pt plus 4pt minus 3pt}

%%%%%%%%%%%%% Footnotes  %%%%%%%%%%%%%%%%%%%%%%%%%
\footmarkwidth=1.2em\footmarksep=0pt
\footmarkstyle{\textsuperscript{#1}\hskip1pt\relax}


%%%%%%%%%%%%%%%%% Papers %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcounter{paper}

\newlength{\afterpaperskip}
\setlength{\afterpaperskip}{10mm}
\newcommand\paperheadname{Paper}
\newcommand\papernumfont{\fontsize{20}{24pt}\usefont{T1}{pbk}{l}{n}}
\newcommand\papertitlefont{\huge\bfseries}

\def\@paper[#1]#2{%
  \def\p@title{#2}
  \def\p@head{#1}
  \def\p@toc{#2}
  \ifx\p@head\@empty %no optional arg
    \def\p@head{#2}
  \fi
  \par
  \clearforchapter
  \thispagestyle{chapter}
  \refstepcounter{paper}
  \addcontentsline{toc}{paper}{%
 %   \protect\numberline{\paperheadname\ \thepaper}\p@toc}%
    \protect\chapternumberline{\thepaper}\p@toc}%
  \markboth{\paperheadname\ \thepaper.\quad\p@head}{}
  \begingroup
    \offinterlineskip
    \hrule height 0pt
    \papernumfont
    \leftline{\paperheadname\ \thepaper}%
  \endgroup
    \afterchapternum
  \begingroup
      \centering
      \papertitlefont \p@title\par
  \endgroup
    \vskip\afterpaperskip
    \newrefsection
}

\def\authors#1{%
  \begin{center}\def\and{\unskip\quad}%
     \large\bfseries#1\par
  \end{center}
  \vspace{10mm}%
}

\def\setP@perCounters{%
  \let\save@elt\@elt
  \let\@elt\setChapter@elt
  \cl@chapter
  \let\@elt\save@elt
}
\def\setChapter@elt#1{\expandafter\def\csname
  the#1\endcsname{\arabic{#1}}}

%\def\l@paper#1#2{\vskip \cftbeforechapterskip 
% \@dottedtocline{0}{0em}{6em}{{\bfseries #1}}{{\bfseries#2}}}

%\newdimen\paper@numlength
\newcommand*{\l@paper}[2]{%
  \begingroup
  % \def\chapternumberlinehook##1{%
  %    \settowidth{\cftchapternumwidth}{##1.}%
  % }
  \renewcommand\cftchapterpresnum{\ }
  \setlength{\cftchapternumwidth}{2.5em}
  \renewcommand{\@tocrmarg}{2.55em plus1fil}
  \l@chapapp{#1}{#2}{Paper}
  \endgroup
  }


\def\paper@label#1{\@memoir@label{@p\thepaper @#1}}
\def\paper@ref#1{\@memoir@ref{@p\thepaper @#1}}
\def\paper@thref#1{\@memoir@thref{@p\thepaper @#1}}
\def\paper@pageref#1{\@memoir@pageref{@p\thepaper @#1}}
\def\paper@label@in@display#1{\@memoir@label@in@display{@p\thepaper @#1}}
\def\thepaper{\Roman{paper}}


\newenvironment{papers}{%
  \setP@perCounters
  \renewcommand{\sectiondot}{.}
  \renewcommand{\captiondot}{.}
  \def\thesubsection{\thesection.\arabic{subsection}}
  \setsecnumdepth{subsection}
  \let\cl@paper\cl@chapter
  \def\paper{\@dblarg\@paper}
%  \let\paper\@paper
  \let\@memoir@label\label
  \let\label\paper@label
  \let\globalLabel\@memoir@label
  \let\@memoir@label@in@display\label@in@display
  \let\label@in@display\paper@label@in@display
  \let\@memoir@ref\ref
  \let\ref\paper@ref
  \let\@memoir@thref\thref
  \let\thref\paper@thref
  \let\@memoir@pageref\pageref
  \let\pageref\paper@pageref
  \openright
}{\par\endrefsection}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%% ntheorem %%%%%%%%%%%%%%%%%%%%%%

\PassOptionsToPackage{amsmath,thmmarks,thref,framed}{ntheorem}
\RequirePackage{ntheorem}
\RequirePackage{latexsym}
\RequirePackage{framed}
\newtheoremstyle{regler}%
  {\item[\rlap{\vbox{\hbox{\hskip\labelsep \theorem@headerfont
          ##1\ ##2\theorem@separator}\hbox{\strut}}}]}%
  {\item[\strut]{\theorem@headerfont##1\ ##2---##3}\newline}

\newtheoremstyle{proof}%
  {\item[\theorem@headerfont\hskip\labelsep ##1\theorem@separator]}%
  {\item[\theorem@headerfont\hskip \labelsep ##1\ ##3\theorem@separator]}
\theoremheaderfont{\bfseries\itshape}
\theorembodyfont{\normalfont}
%\RequirePackage{amssymb}
\theoremstyle{proof}
\theoremsymbol{\ensuremath{\Box}}
\newtheorem{proof}{Proof}

\namerefoff

\theoremsymbol{\ensuremath{\Box}}
\theorembodyfont{\normalfont}
\theoremheaderfont{\scshape}
\def\theoremhack{\NoEndMark\hrule height 0pt\kern-\belowdisplayskip
  \strut\hfill \csname\InTheoType Symbol\endcsname}
\theoremstyle{regler}

%%%%%%%%%%%%% Title Pages %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\newenvironment{titlepages}{
  \newcommand\@month@{}
  \newcommand\@year@{}
  \newcommand\@printer@{}
  \newcommand*\@dedication@{}
  \newcommand*\@type@{MSc Thesis}
  \newcommand*\@ISBN@print{}
  \newcommand*\@ISBN@web{}
  \renewcommand\month[1]{\renewcommand\@month@{##1}}
  \renewcommand\year[1]{\renewcommand\@year@{##1}}
  \renewcommand\title[1]{\newcommand\@title@{##1}}
  \renewcommand\author[1]{\newcommand\@author@{##1}}
  \newcommand\TFRT[1]{\newcommand\@TFRT@{##1}}
  \newcommand*\ThesisType[1]{\renewcommand*\@type@{##1}}
  \newcommand*\ISBNprint[1]{\renewcommand*\@ISBN@print{##1}}
  \newcommand*\ISBNweb[1]{\renewcommand*\@ISBN@web{##1}}
  \newcommand\dedication[1]{\renewcommand*\@dedication@{##1}}
  \newcommand\printer[1]{\renewcommand\@printer@{ by ##1}}
%  \let\@dedication@\strut
}{ 
%  \renewcommand\rmdefault{pbk}\fontfamily{\rmdefault}\selectfont
  % \thispagestyle{empty}
  % \hrule height 0pt
  % \vspace*{1cm}
  % \begin{center}\sffamily\Large\@title@\end{center}%
  % \cleartorecto
  \thispagestyle{empty}
  \vspace*{0pt plus 0.1fill}
  \begin{center}\sffamily
    \huge \@title@\\[0pt plus 0.3fill]
    \LARGE \@author@\\[0pt plus 0.6fill]
    \normalsize
    \includegraphics[width=25mm]{LUBWsv}\\[\medskipamount]
    Department of Automatic Control
  \end{center}
  \newpage
  \thispagestyle{empty}
  \vspace*{0pt plus 0.6fill}%
  \begin{flushleft}
   \@type@\\
   ISRN LUTFD2/TFRT-\hspace{0.1mm}-\@TFRT@-\hspace{0.1mm}-SE\\
   \ifx\@ISBN@print\empty\else ISBN \@ISBN@print\ (print)\\\fi
   \ifx\@ISBN@web\empty\else ISBN \@ISBN@web\ (web)\\\fi
   ISSN 0280--5316

   \vspace{10mm}
   Department of Automatic Control\\
   Lund University\\
   Box 118\\
   SE-221 00 LUND\\
   Sweden\\
   \vspace{10mm}
   \copyright{} \@year@{} by \@author@. All rights reserved.\\
   Printed in Sweden\@printer@.\\
   Lund \@year@
 \end{flushleft}
 \cleartorecto
 \ifx\@dedication@\empty
 \else
   \thispagestyle{empty}
   \vspace*{0pt plus 0.4fill}
   \begin{center}
     \itshape \@dedication@ 
   \end{center}%
   \vspace*{0pt plus 0.6fill}
   \cleartorecto
 \fi
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\frenchspacing\tolerance=1000\emergencystretch=1em
\setlength{\@fptop}{5pt}


%%%%%%%%%%%%% biblatex %%%%%%%%%%%%%%%%%%
\ifno@biblatex \endinput \fi
\renewcommand\memUChead[1]{#1}


\PassOptionsToPackage{style=authoryear}{biblatex}
\PassOptionsToPackage{backend=bibtex}{biblatex}

\RequirePackage{biblatex}
\ExecuteBibliographyOptions{maxcitenames=2,maxbibnames=99,dashed=false}
\ExecuteBibliographyOptions{firstinits=true,uniquename=init}
\AtEveryBibitem{\clearfield{month}}

\let\cite=\parencite
\newcommand\mkciteparens[1]{{[#1]}}
\DeclareCiteCommand{\parencite}[\mkciteparens]%
  {\usebibmacro{prenote}}
  {\usebibmacro{citeindex}%
   \usebibmacro{cite}}
  {\multicitedelim}
  {\usebibmacro{postnote}}
\renewcommand*{\nameyeardelim}{\addcomma\addspace}
 
\renewcommand*{\bibsetup}{%
  \interlinepenalty=5000\relax
  \widowpenalty=10000\relax
  \clubpenalty=10000\relax
  \raggedbottom
  \frenchspacing
  \tolerance=2000
  \emergencystretch=2em
  \setlength{\bibitemsep}{\smallskipamount}
  \biburlsetup}


% \renewbibmacro*{name:andothers}{%
%   \ifboolexpr{
%     test {\ifnumequal{\value{listcount}}{\value{liststop}}}
%     and
%     test \ifmorenames
%   }
%     {\ifnumgreater{\value{liststop}}{1}
%        {\finalandcomma}
%        {}%
%      \andothersdelim\bibstring[\mkbibemph]{andothers}}
%     {}}



% We want to remove an 'in:' from the article entries. Unfortunately
% this requires a complete copy of the Driver.
\DeclareBibliographyDriver{article}{%
  \usebibmacro{bibindex}%
  \usebibmacro{begentry}%
  \usebibmacro{author/translator+others}%
  \setunit{\labelnamepunct}\newblock
  \usebibmacro{title}%
  \newunit
  \printlist{language}%
  \newunit\newblock
  \usebibmacro{byauthor}%
  \newunit\newblock
  \usebibmacro{bytranslator+others}%
  \newunit\newblock
  \printfield{version}%
  \newunit\newblock
  %\usebibmacro{in:}%
  \usebibmacro{journal+issuetitle}%
  \newunit
  \usebibmacro{byeditor+others}%
  \newunit
  \usebibmacro{note+pages}%
  \newunit\newblock
  \iftoggle{bbx:isbn}
    {\printfield{issn}}
    {}%
  \newunit\newblock
  \usebibmacro{doi+eprint+url}%
  \newunit\newblock
  \usebibmacro{addendum+pubstate}%
  \setunit{\bibpagerefpunct}\newblock
  \usebibmacro{pageref}%
  \newunit\newblock
  \usebibmacro{related}%
  \usebibmacro{finentry}}

% We want to include a number for the theses. Unfortunately this
% requires a complete copy of the driver.
\DeclareBibliographyDriver{thesis}{%
  \usebibmacro{bibindex}%
  \usebibmacro{begentry}%
  \usebibmacro{author}%
  \setunit{\labelnamepunct}\newblock
  \usebibmacro{title}%
  \newunit
  \printlist{language}%
  \newunit\newblock
  \usebibmacro{byauthor}%
  \newunit\newblock
  \printfield{note}%
  \newunit\newblock
  \printfield{type}%
  \setunit*{\addspace}%
  \printfield{number}% Added locally
  \newunit
  \usebibmacro{institution+location+date}%
  \newunit\newblock
  \usebibmacro{chapter+pages}%
  \newunit
  \printfield{pagetotal}%
  \newunit\newblock
  \iftoggle{bbx:isbn}
    {\printfield{isbn}}
    {}%
  \newunit\newblock
  \usebibmacro{doi+eprint+url}%
  \newunit\newblock
  \usebibmacro{addendum+pubstate}%
  \setunit{\bibpagerefpunct}\newblock
  \usebibmacro{pageref}%
  \newunit\newblock
  \usebibmacro{related}%
  \usebibmacro{finentry}}

% Make theses look like book titles
\DeclareFieldFormat[thesis]{title}{\mkbibemph{#1}}

% The original version has the form vol.num.
% we want <bold>vol</bold>:num
\renewbibmacro*{volume+number+eid}{%
  \printfield[bold]{volume}%
  \setunit*{\addcolon}%
  \printfield{number}%
  \setunit{\addcomma\space}%
  \printfield{eid}}

% The original version had the form address: publisher.
% We want publisher, address.
\renewbibmacro*{publisher+location+date}{%
  \printlist{publisher}%
  \setunit*{\addcomma\space}%
  \printlist{location}%
  \setunit*{\addcomma\space}%
  \usebibmacro{date}%
  \newunit}

% The original version had the form address: institution.
% We want institution, address.
\renewbibmacro*{institution+location+date}{%
  \printlist{institution}
  \setunit*{\addcomma\space}%
  \printlist{location}%
  \setunit*{\addcomma\space}%
  \usebibmacro{date}%
  \newunit}

% Make the titles of articles and conference contributions 'sentence case',
% but don't touch any other titles.
% http://tex.stackexchange.com/questions/22980/sentence-case-for-titles-in-biblatex
\DeclareFieldFormat{titlecase}{\MakeTitleCase{#1}}
\newrobustcmd{\MakeTitleCase}[1]{%
  \ifthenelse{\ifcurrentfield{booktitle}\OR\ifcurrentfield{booksubtitle}%
    \OR\ifcurrentfield{maintitle}\OR\ifcurrentfield{mainsubtitle}%
    \OR\ifcurrentfield{journaltitle}\OR\ifcurrentfield{journalsubtitle}%
    \OR\ifcurrentfield{issuetitle}\OR\ifcurrentfield{issuesubtitle}%
    \OR\ifentrytype{book}\OR\ifentrytype{mvbook}\OR\ifentrytype{bookinbook}%
    \OR\ifentrytype{booklet}\OR\ifentrytype{suppbook}%
    \OR\ifentrytype{collection}\OR\ifentrytype{mvcollection}%
    \OR\ifentrytype{suppcollection}\OR\ifentrytype{manual}%
    \OR\ifentrytype{periodical}\OR\ifentrytype{suppperiodical}%
    \OR\ifentrytype{proceedings}\OR\ifentrytype{mvproceedings}%
    \OR\ifentrytype{reference}\OR\ifentrytype{mvreference}%
    \OR\ifentrytype{report}\OR\ifentrytype{thesis}}
    {#1}
    {\MakeSentenceCase{#1}}}

% We want incollection and inbook entries to have editor specifications like:
% In: Editor E. (Ed.). Booktitle. Publisher. Address
%
\newbibmacro*{ineditor+others}{%
  \ifboolexpr{ test \ifuseeditor and not test {\ifnameundef{editor}} }
    {\defcounter{maxnames}{1}% Only show one editor, use et al. if more.
      \printnames{editor}%
     \setunit{\addspace}%
     \usebibmacro{editor+othersstrg}%
     \clearname{editor}}
    {}}

% Print editors before "in" title
\renewbibmacro{in:}{%
  \ifentrytype{article}{}{\printtext{\bibstring{in}\intitlepunct}}%
  \usebibmacro{ineditor+others}%
  \newunit
  \clearname{editor}}

\DeclareFieldFormat{editortype}{\mkbibparens{#1}}

\DefineBibliographyStrings{english}{% various adjustments to
                                    % common bib entry strings
  editor = {Ed.},
  editors = {Eds.},
}

