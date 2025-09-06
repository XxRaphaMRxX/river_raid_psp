#!/bin/bash

# Script de instalao do ambiente PSPDev no WSL (Ubuntu)

echo "Atualizando pacotes e instalando dependncias..."
sudo apt update && sudo apt install build-essential git wget tar make cmake python3 -y

echo "Criando diretrio de instalao /opt/pspdev..."
sudo mkdir -p /opt/pspdev
sudo chown $(whoami):$(whoami) /opt/pspdev

echo "Extraindo o toolchain PSPDev para /opt/pspdev..."
# O caminho para o arquivo .tar.gz no Windows  acessvel via /mnt/d no WSL
tar -zxvf "/mnt/d/Meus_Projetos/pspdev/pspdev-ubuntu-latest-x86_64.tar.gz" -C /opt/pspdev

echo "Configurando variveis de ambiente no ~/.bashrc..."
# Adiciona as variveis de ambiente se ainda no existirem
if ! grep -q "export PSPDEV=/opt/pspdev" ~/.bashrc;
then
    echo -e "\n# PSPDev Environment Variables" >> ~/.bashrc
    echo "export PSPDEV=/opt/pspdev" >> ~/.bashrc
    echo "export PATH=\$PATH:\$PSPDEV/bin" >> ~/.bashrc
    echo "As variveis de ambiente foram adicionadas ao ~/.bashrc."
    echo "Por favor, execute 'source ~/.bashrc' ou reinicie seu terminal para que as mudanças tenham efeito."
else
    echo "As variveis de ambiente PSPDev j esto configuradas no ~/.bashrc."
fi

echo "Instalao do PSPDev concluda!"
echo "Lembre-se de executar 'source ~/.bashrc' ou reiniciar seu terminal para que as variveis de ambiente sejam carregadas."
