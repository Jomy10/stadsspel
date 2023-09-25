#/usr/bin/env zsh

cd "$(dirname "$0")"

cmd=$1
if [[ $cmd == "" ]]; then
  cmd=$(gum choose "build" "install" "gen-key" "emulator")
fi

case $cmd in
  build)
    sh dev-util/build_app.sh 2>&1 #| ruby dev-util/in_colorize.rb
    ;;
  install)
    # Installs the app on the running simulator
    sh dev-util/install_app.sh
    ;;
  gen-key)
    ty=$(gum choose "debug" "release")
    echo "Choose a password for the key"
    pass=$(gum input --placeholder "android")
    STOREPASS=$pass sh dev-util/gen_key.sh $ty
    ;;
  emulator)
    action=$(gum choose "start" "logcat")
    case $action in
      start)
        sh dev-util/emulator_start.sh
        ;;
      logcat)
        sh dev-util/emulator_log.sh | rg stadsspel | ruby dev-util/log_colorizer.rb
        ;;
    esac
    ;;
esac
