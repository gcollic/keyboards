docker build -t temp .
$id=(docker create temp)
docker cp "$($id):/src/keyboard_firmware.hex" .
docker rm -v $id