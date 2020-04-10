#!

echo "perl LatexDiff/latexpand.pl memoire.tex > flat.tex"
perl LatexDiff/latexpand.pl memoire.tex > flat.tex

echo "cd VersionAComparer"
cd VersionAComparer

echo "perl ../LatexDiff/latexpand.pl memoire.tex > flat.tex"
perl ../LatexDiff/latexpand.pl memoire.tex > flat.tex

cd ..

echo "latexdiff -c ld.cfg flat.tex VersionAComparer/flat.tex >diff.tex"
latexdiff -c LatexDiff/ld.cfg flat.tex VersionAComparer/flat.tex >diff.tex

pdflatex diff


