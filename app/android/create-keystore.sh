keytool -genkey -v -keystore my-release-key.keystore -alias androidkey \
  -keyalg RSA -keysize 2048 -validity 10000 \
  -storepass mypass123 \
  -keypass mypass123 \
  -dname "CN=jonaseveraert.be, OU=ID, O=jonaseveraert, L=De Haan, S=West-Vlaanderen, C=BE"
