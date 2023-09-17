ty=$1
if [[ $ty == "" ]]; then
  ty=debug
fi

if [[ $STOREPASS == "" ]]; then
  echo "Choose a password for the key"
  STOREPASS=$(gum input --placeholder android)
fi

[ -f "${ty}.keystore" ] && rm "${ty}.keystore"

keytool -genkey -v \
  -keystore "${ty}.keystore" \
  -alias androidkey \
  -keyalg RSA \
  -keysize 2048 \
  -validity 10000 \
  -storepass ${STOREPASS} \
  -keypass ${STOREPASS}
  # -dname androidkey
