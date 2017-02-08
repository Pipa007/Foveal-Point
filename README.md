# Foveal-Point

O objectivo é receber uma imagem e ter à saída a imagem foveada.
A input image tem uma bounding box associada e o foveated point é o centro dessa caixa.

O tamanho da fovea, sigma, varia para 3, 10 e 25.


Para compilar:
root/

mkdir build
cd build

cmake..
make


Para executar:
root/
bash scripts/config.sh
