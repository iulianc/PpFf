#

COMMIT_DEFAUT="f7ae18e169edc699bf8cc4c39e2960fe58715dc3"

if [[ $# == 0 ]]; then
    COMMIT="$COMMIT_DEFAUT"
else
    COMMIT="$1"
fi

echo "On checkout le commit $COMMIT"
git co $COMMIT

echo "On copie les fichiers"
\cp -f introduction.tex VersionAComparer
\cp -f remerciements.tex VersionAComparer
\cp -f resume.tex VersionAComparer
\cp -f outils_connus.tex VersionAComparer
\cp -f description.tex VersionAComparer
\cp -f implementation.tex VersionAComparer
\cp -f experiences.tex VersionAComparer
\cp -f conclusion.tex VersionAComparer

echo "On retourne a master"
git co master
