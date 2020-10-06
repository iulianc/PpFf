%
% format LaTeX pour memoire et these de l'UQAM
% d'apres "Guide de presentation des memoires et theses"
% auteur: Gisele Legault
% collaborateurs: Stephane Poirier, Rene Ferland,
%                  Luc Belanger, Andre Lauzon
%                  Jerome Tremblay, Anthony Monnet

%  for dull, boring thesis format:
%       \documentclass[12pt]{memoireuqam1.3}
%

% Modifications:
% aout 2013
% commande UpperRef defini en fonction du chargement du package hypperref
% ajout \matiere pour page titre  - Anthony Monnet
% vrification chargement package natbib. Si oui, utiliser la \bibitem de natbib
% sinon utiliser \bibitem de memoireuqam1.3
% octobre 2012
% Ajouté des commandes du package setspace pour avoir double
% interligne dans la page titre
% avril 2012
% Remplacer \par par \\ dans la page titre pour avoir la même interligne partout
% Mis la liste des tableaux et la liste des figures en simple interligne
% juin 2011
% Ajouté Figure + page dans la liste des figures
% Ajouté Tableau + page dans la liste des tableaux
% Octobre 2011 - Gisèle Legault
% corrigé erreur avec la \paragraph et \subparagraph
% Novembre 2010 - tremblay.jerome@uqam.ca
%   Enlevé le gras dans les titres
%   Ne pas paginer les pages vides
% janvier 2007
% ajouté \evensidemargin pour impression recto-verso
% octobre 2004
%1) Mis les noms des sections en majuscules pour utiliser le package babel
%2) enlever definition \og et \fg (redefini par babel)
%3) ajouter \uqammemoire, \uqamthese et \uqamrapport pour la page titre

% Un de mes ajouts (Luc Belanger)
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{latex/memoireuqam1.3}
              [2013/08/28 v1.3g
 Format LaTeX pour les memoires a l'UQAM.]

% Un autre ajout pour enlever le warning (Luc Belanger)
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}
\ProcessOptions
\LoadClass{report}



\def\mystretch{1.5}             % interligne et demi


\def\baselinestretch{\mystretch}        % interligne et demi

\newlength{\simpleinter}
\setlength{\simpleinter}{\baselineskip}
\newlength{\doubleinter}
\setlength{\doubleinter}{1.5\baselineskip plus 1mm minus 1mm}

%%%%%%%  Set up margins and formatting params %%%


%%%%%%%%%%%%%%%%%%%%%%
% Marges
%%%%%%%%%%%%%%%%%%%%%%
\setlength{\oddsidemargin}{1.35cm}      % 4cm left margin
\setlength{\evensidemargin}{0.35cm}
\setlength{\topmargin}{1.3cm}           % 4cm top margin
\setlength{\textwidth}{14.6cm}          % 6.0in text - 1.25in rt margin
\setlength{\textheight}{21.2cm} % Body ht for 1in margins
\addtolength{\topmargin}{-\headheight}  % No header, so compensate
\addtolength{\topmargin}{-\headsep}     % for header height and separation

%%%%%%%  End of margins and formatting params %%%


% Novembre 2010 - Ne pas paginer les pages vides (tremblay.jerome@uqam.ca)
% Code for creating empty pages
% No headers on empty pages before new chapter
\def\cleardoublepage{\clearpage\if@twoside \ifodd\c@page\else
    \hbox{}
    \thispagestyle{empty}
    \newpage
    \if@twocolumn\hbox{}\newpage\fi\fi\fi}

\edef\partpdf#1\expandafter{#1}

\AtBeginDocument{
   \@ifpackageloaded{hyperref}
   {\newcommand{\UpperRef}[1]{\texorpdfstring{\MakeUppercase{#1}}
                   { \expandafter\expandafter\partpdf\expandafter{#1} } }}
   {\newcommand{\UpperRef}[1]{\MakeUppercase{#1}}}

%\@ifpackageloaded{babel}
%{\@ifpackagewith{babel}{french}
%{\shorthandoff{~}}
%{\relax}
%}
%{\relax}

% \@ifpackageloaded{natbib}
%    {\relax}
%    {
%
% \def\@citex[#1]#2{%
%   \let\@citea\@empty
%   \@cite{\@for\@citeb:=#2\do
%     {\@citea\def\@citea{;\penalty\@m\ }%
%      \edef\@citeb{\expandafter\@firstofone\@citeb}%
%      \if@filesw\immediate\write\@auxout{\string\citation{\@citeb}}\fi
%      \@ifundefined{b@\@citeb}{\mbox{\reset@font\bfseries ?}%
%        \G@refundefinedtrue
%        \@latex@warning
%          {Citation `\@citeb' on page \thepage \space undefined}}%
%        {\csname b@\@citeb\endcsname}}}{#1}}
%
%
% \def\hang{\hangindent\parindent}
% \def\@cite#1#2{({#1\if@tempswa , #2\fi})}
%
% \def\@biblabel#1{(#1)\hfill}
% %\def\@lbibitem[#1]#2{\item[{}]\if@filesw
% % \def\@lbibitem[#1]#2{\item[{}]\ \par\if@filesw
% %       { \def\protect##1{\string ##1\space}\immediate
% %         \write\@auxout{\string\bibcite{#2}{#1}}}\fi\ignorespaces }
%
% \def\bibitem{\hang \@ifnextchar[{\@lbibitem}{\@bibitem}}
%
% \newdimen\bibindent
% \bibindent=1.5em
% \renewenvironment{thebibliography}[1]
%      {\chapter*{\UpperRef{\bibname}
%         \@mkboth{\UpperRef{\bibname}}{\UpperRef{\bibname}}}%
%       \addtocontents{toc}{\protect\vspace{1.5ex}}
%       \addcontentsline{toc}{chapter}{\UpperRef{\bibname}}
%       \baselineskip=\simpleinter
%       \list{\@biblabel{\arabic{enumiv}}}%
%            {\settowidth\labelwidth{\@biblabel{#1}}%
%             \settowidth{\leftmargin}{rr}
%             %\leftmargin\labelwidth
%             \advance\leftmargin\labelsep
%             \@openbib@code
%             \setlength{\listparindent}{-\leftmargin}
%             \setlength{\itemsep}{-0.5em}
%             \setlength{\parsep}{0em}
%             \usecounter{enumiv}%
%             \let\p@enumiv\@empty
%             \renewcommand\theenumiv{\arabic{enumiv}}}%
%       \sloppy\clubpenalty4000
%       \@clubpenalty \clubpenalty
%       \widowpenalty4000%
%       \sfcode`\.=\@m}
%      {\def\@noitemerr
%        {\@latex@warning{Empty `thebibliography' environment}}%
%       \endlist}
%
%    }
}


% Redefine '/chapter' to always start on an odd page.
% Should make no difference in singleside mode.
%
\def\chapter{\cleardoublepage   % Starts new page.
   \thispagestyle{empty}        % Page non numerotee
   \global\@topnum\z@           % Prevents figures from going at top of page.
   \@afterindenttrue            % indent in first paragraph.  Change
   \secdef\@chapter\@schapter}  % to \@afterindentfalse to have none.




% If using the report style, use - instead of . in the figure number.

\newcommand\numerochap{\Roman{chapter}}
\renewcommand\thechapter                {\arabic{chapter}}
\renewcommand\thesection                {\thechapter.\arabic{section}}
\@ifundefined{thechapter}{}{\def\thefigure{\thechapter-\arabic{figure}}}

\def\@makechapterhead#1{%
  {\parskip=0pt
   \vspace*{0.3 truecm}
   \parindent \z@ \raggedright \reset@font
    \ifnum \c@secnumdepth >\m@ne
         \centering {\expandafter\MakeUppercase\expandafter{\large \@chapapp{} \numerochap}} \par
         \vskip 1.6truecm
       \fi
    \centering {\MakeUppercase{ \large #1}}\par
    \nobreak
    \vspace*{2.15 truecm}
  \parskip=2ex}}

\def\@makeschapterhead#1{
   \parskip=0pt
   \vspace*{0.1 truecm}
   \begin{center}
    {\MakeUppercase{\large #1 }}\par
   \end{center}
   \nobreak \vspace*{1.10 truecm}
   \parskip=2ex  }


%%%%%%%%%%%%%%%%
% FRANCISATION DES TERMES
%%%%%%%%%%%%%%%%

\renewcommand\contentsname{TABLE DES MATI\`ERES}
\renewcommand\listfigurename{LISTE DES FIGURES}
\renewcommand\listtablename{LISTE DES TABLEAUX}
\renewcommand\bibname{BIBLIOGRAPHIE}
\renewcommand\indexname{INDEX}
\renewcommand\figurename{Figure}
\renewcommand\tablename{Tableau}
\renewcommand\partname{PARTIE}
\renewcommand\chaptername{CHAPITRE}
\renewcommand\appendixname{APPENDICE}
\renewcommand\abstractname{R\'ESUM\'E}

%%%%%%%%%%%%%%%%%%%
%% INDENTATION DES SECTIONS
%%%%%%%%%%%%%%%%%%%

\renewcommand\section{\@startsection{section}{1}{\z@}%
                                   {1.5ex \@plus .2ex \@minus .2ex}%
                                   {1ex \@plus.2ex}%
                                   {\normalfont\large}}
\renewcommand\subsection{\@startsection{subsection}{2}{\z@}%
                                     {1.5ex\@plus .2ex \@minus .2ex}%
                                     {1ex \@plus .2ex}%
                                     {\normalfont\large}}
\renewcommand\subsubsection{\@startsection{subsubsection}{3}{\z@}%
                                     {1.5ex\@plus .2ex \@minus .2ex}%
                                     {1ex \@plus .2ex}%
                                     {\normalfont\normalsize}}
\renewcommand\paragraph{\@startsection{paragraph}{4}{\z@}%
                                    {3.25ex \@plus1ex \@minus.2ex}%
                                    {-1em}%
                                    {\normalfont\normalsize}}
\renewcommand\subparagraph{\@startsection{subparagraph}{5}{\z@}%
                                       {3.25ex \@plus1ex \@minus .2ex}%
                                       {-1em}%
                                      {\normalfont\normalsize}}


\markright{}
\pagestyle{myheadings}
\pagenumbering{arabic}      % Arabic page numbers
%%%%%%% pas de renfoncement au début d'un paragraphe
\parindent=0ex

\parskip=2ex


%%%%%%%%%%%%
%%
%%  FIGURES ET TABLES
%%
%%%%%%%%%%%%
\setlength\abovecaptionskip{10\p@}
\setlength\belowcaptionskip{5\p@}


\long\def\@makecaption#1#2{%
  \vskip\abovecaptionskip
  \sbox\@tempboxa{{\bf #1} #2}%
  \ifdim \wd\@tempboxa >\hsize
    {\bf #1} #2\par
  \else
    \global \@minipagefalse
    \hb@xt@\hsize{\hfil\box\@tempboxa\hfil}%
  \fi
  \vskip\belowcaptionskip}


\renewcommand\thefigure{{\thechapter.\@arabic\c@figure}}
\def\fps@figure{tbp}
\def\ftype@figure{1}
\def\ext@figure{lof}
\def\fnum@figure{{\figurename~}\thefigure}
\renewenvironment{figure}
               {\@float{figure}}
               {\end@float}
\renewenvironment{figure*}
               {\@dblfloat{figure}}
               {\end@dblfloat}

\renewcommand\thetable{{\thechapter.\@arabic\c@table}}
\def\fps@table{tbp}
\def\ftype@table{2}
\def\ext@table{lot}
\def\fnum@table{{\tablename~}\thetable}
\renewenvironment{table}
               {\@float{table}}
               {\end@float}
\renewenvironment{table*}
               {\@dblfloat{table}}
               {\end@dblfloat}

%%%%%%%%%%%%%%%%%
%%
%% TABLE DES MATIERES
%%
%%%%%%%%%%%%%%%%%

\newcommand\l@specialchapitre {\@dottedtocline{1}{0em}{0em}}

\renewcommand\tableofcontents{%
    \thispagestyle{empty}
    \baselineskip=\simpleinter
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\expandafter\MakeUppercase\expandafter{\contentsname}}
    \parskip=0ex
    \vspace*{-1.5ex}
    \@starttoc{toc}%
    \noindent
    \if@restonecol\twocolumn\fi
    \baselineskip=\doubleinter
    \parskip=2ex}

\renewcommand\l@chapter[2]{%
  \ifnum \c@tocdepth >\m@ne
    \addpenalty{-\@highpenalty}%
    \vskip 1.0em \@plus\p@
    \setlength\@tempdima{1.5em}%
    \begingroup
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      \nopagebreak
      \leavevmode %\bfseries
      \hskip -\leftskip
%      \@dottedtocline{1}{0em}{2.3em}
      #1\nobreak\hfil \nobreak\hbox to\@pnumwidth{\hss #2}\par
      \penalty\@highpenalty
    \endgroup
  \fi}

\def\@chapter[#1]#2{\ifnum \c@secnumdepth >\m@ne
                         \refstepcounter{chapter}%
                         \typeout{\@chapapp\space\thechapter.}%
                       % Chapitre et titre sur la meme ligne G.L. 23/11/2018
                         \addtocontents{toc}{\protect\vspace{1.5ex}}
                         \addcontentsline{toc}{chapter}{\UpperRef{\@chapapp\protect\hskip 0.5em \numerochap} \protect\hskip 0.5em \UpperRef{#1}} 
                     \else
                      \addcontentsline{toc}{chapter}{#1}
                    \fi
                    \chaptermark{#1}%
                    \if@twocolumn
                      \@topnewpage[\@makechapterhead{#2}]%
                    \else
                      \@makechapterhead{#2}%
                      \@afterheading
                    \fi
                    }

\def\@sect#1#2#3#4#5#6[#7]#8{\ifnum #2>\c@secnumdepth
      \let\@svsec\@empty \else
    \refstepcounter{#1} \protected@edef\@svsec{\@seccntformat{#1}\hskip
1em}%
  \fi
  \@tempskipa #5\relax
  \ifdim \@tempskipa>\z@
    \begingroup #6\relax

        \@hangfrom{\hskip #3\relax\@svsec} {\interlinepenalty \@M #8\@@par}%
    \endgroup
    \csname #1mark\endcsname{#7}%
    \addtocontents{toc}{\protect\vspace{1.5ex}}
    \addcontentsline
    {toc}{#1}{\ifnum #2>\c@secnumdepth \else
        \protect\numberline{\csname the#1\endcsname}%
      \fi #7} \else
    \def\@svsechd{#6\hskip #3\relax
       \@svsec #8}\csname #1mark\endcsname
        {#7} \addcontentsline
         {toc}{#1}{\ifnum #2>\c@secnumdepth \else
          \protect\numberline{\csname
           the#1\endcsname}%
        \fi }

  \fi
  \@xsect{#5}}

\renewcommand\l@chapter      {\@dottedtocline{0}{0em}{0em}}
\renewcommand\l@section      {\@dottedtocline{1}{0em}{2.3em}}
\renewcommand\l@subsection   {\@dottedtocline{2}{2.3em}{3.2em}}
\renewcommand\l@subsubsection{\@dottedtocline{3}{7.0em}{4.1em}}

%%%%%%%%%%%%%%%%%
%%
%% ENVIRONNEMENT APPENDICE
%%
%%%%%%%%%%%%%%%%%

\def\appendix{\par
  \setcounter{chapter}{0}%
  \setcounter{section}{0}%
  % modifie \MakeUppercase par \UpperRef   G.L. 28/08/2013
  \def\@chapapp{\UpperRef{\appendixname}}%
  \renewcommand\numerochap{\Alph{chapter}}
  \def\thechapter{\Alph{chapter}}}

%%%%%%%%%%%%%%%%%
%%
%% LISTE DES FIGURES
%%
%%%%%%%%%%%%%%%%%

\renewcommand\listoffigures{%
{
    \baselineskip=\simpleinter
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
  % modifie \MakeUppercase par \UpperRef   G.L. 28/08/2013
    \chapter*{\UpperRef{\listfigurename}
      \@mkboth{\UpperRef{\listfigurename}}%
              {\UpperRef{\listfigurename}}}%
    \addtocontents{toc}{\protect\vspace{1.5ex}}
    \addcontentsline{toc}{chapter}{\UpperRef{\listfigurename}}
    \noindent Figure \hfill Page
    \@starttoc{lof}%
    \if@restonecol\twocolumn\fi
    }
    }


%%%%%%%%%%%%%%%%%
%%
%% LISTE DES TABLEAUX
%%
%%%%%%%%%%%%%%%%%

\renewcommand\listoftables{%
{
    \baselineskip=\simpleinter

    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
      % modifie \MakeUppercase par \UpperRef   G.L. 28/08/2013
    \chapter*{\UpperRef{\listtablename}
      \@mkboth{\UpperRef{\listtablename}}{\UpperRef{\listtablename}}}%
    \addtocontents{toc}{\protect\vspace{1.5ex}}
    \addcontentsline{toc}{chapter}{\UpperRef{\listtablename}}
    \noindent Tableau \hfill Page
    \@starttoc{lot}%
    \if@restonecol\twocolumn\fi
    }
}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Footnote
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\setlength{\footnotesep}{\doubleinter}

\renewcommand{\@makefntext}[1]%
  {\setlength{\parindent}{2em}%
   \makebox[1.8em][r]{\@makefnmark}#1}

\renewcommand{\footnoterule}{
 \noindent
 \rule{4cm}{0.4pt}
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Environnement Resume
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  \renewenvironment{abstract}{%
  % modifie \MakeUppercase par \UpperRef   G.L. 28/08/2013
      \chapter*{\UpperRef{\abstractname}}
      \addtocontents{toc}{\protect\vspace{1.5ex}}
      \addcontentsline{toc}{chapter}{\UpperRef{\abstractname}}
     \baselineskip=\simpleinter}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Environnement Introduction
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\newenvironment{introduction}{%
\chapter*{INTRODUCTION}
\pagenumbering{arabic}
\setcounter{page}{1}
\addtocontents{toc}{\protect\vspace{1.5ex}}
\addcontentsline{toc}{chapter}{\UpperRef{INTRODUCTION}}
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Environnement CONCLUSION
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\newenvironment{conclusion}{%
 \chapter*{CONCLUSION}
 \addtocontents{toc}{\protect\vspace{1.5ex}}
 \addcontentsline{toc}{chapter}{CONCLUSION}
}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% PAGE TITRE
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%\newif\ifisthesis
%\global\isthesistrue
\newcommand{\restore@spacing}{%
    \par
    \vskip \parskip
    \vskip \baselineskip
  \endgroup
  \vskip -\parskip
  \vskip -\baselineskip
}

\newcommand{\setstretch}[1]{%
  \def\baselinestretch{#1}%
  \@currsize
}
\newcommand{\doublespacing}{%
  \setstretch {2}%  default
}

% double spacing is 2 x pt size
\newenvironment{doublespace}{%
  \begingroup
    \doublespacing
}{%
  \restore@spacing
}

\def\degree#1{\setbox0\hbox{#1}
\def\@degree{#1}}
\def\degreemonth#1{\gdef\@degreemonth{#1}}
\def\degreeyear#1{\gdef\@degreeyear{#1}}
% Ajout de \matiere  A.M. 28/08/2013
\def\matiere#1{\gdef\@matiere{#1}}
% matiere par defaut = mathematiques
\matiere{MATH\'EMATIQUES}

\def\thesetype{TH\`ESE}
\def\theseprog{DU DOCTORAT EN \expandafter\MakeUppercase\expandafter{\@matiere}}
\def\accord{E}
\def\uqamthese{
   %\isthesistrue
   \gdef\thesetype{TH\`ESE}
   \gdef\theseprog{DU DOCTORAT EN \expandafter\MakeUppercase\expandafter{\@matiere}}
   \gdef\accord{E}
}
\def\uqammemoire{
   %\isthesisfalse
   \gdef\thesetype{M\'EMOIRE}
   \gdef\theseprog{DE LA MA\^ITRISE EN \expandafter\MakeUppercase\expandafter{\@matiere}}
   \gdef\accord{}
}

\def\uqamrapport{
   %\isthesisfalse
   \gdef\thesetype{RAPPORT DE RECHERCHE}
   \gdef\theseprog{DE LA MA\^ITRISE EN \expandafter\MakeUppercase\expandafter{\@matiere}}
   \gdef\accord{}
}


\def\maketitle{\begin{titlepage}
\begin{doublespace}
\begin{center}
\large
UNIVERSIT\'E DU QU\'EBEC \`A MONTR\'EAL\\
\vfill

{\large\MakeUppercase{\@title}}\\
\vfill

\thesetype
\\
PR\'ESENT\'E\accord \\
COMME EXIGENCE PARTIELLE\\
\theseprog\\
\vfill
PAR\\
{\MakeUppercase{\expandafter{\@author}}}
\\
\vfill
\MakeUppercase{\expandafter{\@degreemonth} \@degreeyear}
\\
\end{center}
\end{doublespace}
\end{titlepage}
}
