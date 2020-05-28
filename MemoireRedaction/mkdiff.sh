#!

# Script pour générer le fichier diff.tex à partir du contenu du
# fichier memoire.tex dans le répertoire courant et à partir des
# fichiers *.tex intéressants dans le répertoire VersionAComparer.

#
# Le script co-version-a-comparer.sh peut être appelé pour mettre dans
# le répertoire VersionAComparer <<presque>> n'importe quelle version
# antérieure. Presque => une version postérieur au tag
# PREMIERE_VERSION, commit dans lequel les divers commentaires \gt/ic
# ont été supprimés et tous les caractères spéciaux des lstlisting ont
# été supprimés, assurant ainsi que latexdiff fonctionne correctement.
#


sed  's/^\\include{appendice/%\\include}appendice/' memoire.tex > mem.tex
perl LatexDiff/latexpand.pl mem.tex > flat.tex
\rm -f mem.tex

echo "cd VersionAComparer"
cd VersionAComparer
sed  's/^\\include{appendice/%\\include}appendice/' memoire.tex > mem.tex
perl ../LatexDiff/latexpand.pl mem.tex > flat.tex
\rm -f mem.tex
cd ..

echo "latexdiff -c LatexDiff/ld.cfg VersionAComparer/flat.tex flat.tex >diff.tex"
latexdiff -c LatexDiff/ld.cfg VersionAComparer/flat.tex flat.tex > diff.tex

pdflatex diff
