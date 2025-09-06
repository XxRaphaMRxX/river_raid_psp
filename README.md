# Configuração do Ambiente PSPDev no WSL (Ubuntu)

Este guia detalha como configurar um ambiente de desenvolvimento para PSP (PlayStation Portable) usando o PSPDev no Windows Subsystem for Linux (WSL) com Ubuntu.

## 1. Estrutura Inicial do Projeto

Antes de começar, crie a seguinte estrutura de diretórios dentro do seu diretório home no WSL (por exemplo, `~/psp-project/`). Você pode fazer isso manualmente ou usando os comandos abaixo no seu terminal WSL:

```bash
mkdir -p ~/psp-project/src
mkdir -p ~/psp-project/include
mkdir -p ~/psp-project/assets
mkdir -p ~/psp-project/build
```

Após criar a estrutura, copie os arquivos `main.c` e `Makefile` (que foram gerados na pasta `D:\Meus_Projetos\pspdev`) para o diretório `~/psp-project/` no WSL. O arquivo `main.c` deve ir para `~/psp-project/src/` e o `Makefile` para `~/psp-project/`.

```bash
# Exemplo de como copiar os arquivos (assumindo que você está no diretório home do WSL)
cp /mnt/d/Meus_Projetos/pspdev/main.c ~/psp-project/src/
cp /mnt/d/Meus_Projetos/pspdev/Makefile ~/psp-project/
```

## 2. Instalação do PSPDev no WSL

Para automatizar a instalação do toolchain PSPDev e suas dependências, utilize o script `install_pspdev.sh`.

1.  **Copie o script para o WSL:**
    ```bash
    cp /mnt/d/Meus_Projetos/pspdev/install_pspdev.sh ~/
    ```

2.  **Torne o script executável:**
    ```bash
    chmod +x ~/install_pspdev.sh
    ```

3.  **Execute o script:**
    ```bash
    ~/install_pspdev.sh
    ```
    *   Este script irá:
        *   Atualizar os pacotes do sistema e instalar dependências como `build-essential`, `git`, `wget`, `tar`, `make`, `cmake` e `python3`.
        *   Criar o diretório `/opt/pspdev`.
        *   Extrair o toolchain `pspdev-ubuntu-latest-x86_64.tar.gz` (localizado em `D:\Meus_Projetos\pspdev` no Windows, acessível via `/mnt/d/Meus_Projetos/pspdev` no WSL) para `/opt/pspdev`.
        *   Configurar as variáveis de ambiente `PSPDEV` e `PATH` no seu arquivo `~/.bashrc`.

4.  **Carregue as variáveis de ambiente:**
    Após a execução do script, você precisará carregar as novas variáveis de ambiente. Você pode fazer isso executando:
    ```bash
source ~/.bashrc
```
    Ou simplesmente fechando e reabrindo seu terminal WSL.

## 3. Compilação do Projeto no WSL

Com o ambiente PSPDev configurado e os arquivos `main.c` e `Makefile` no lugar, você pode compilar seu projeto.

1.  **Navegue até o diretório do projeto:**
    ```bash
    cd ~/psp-project/
    ```

2.  **Execute o comando `make`:**
    ```bash
    make
    ```
    *   O `Makefile` irá compilar `main.c` usando `psp-gcc`, gerar um arquivo `.elf` e, em seguida, criar o arquivo `EBOOT.PBP` (o executável do PSP) no diretório `~/psp-project/`.

## 4. Execução no Emulador (PPSSPP no Windows)

Para rodar seu jogo compilado no emulador PPSSPP no Windows, você precisará acessar os arquivos do WSL a partir do Windows.

1.  **Localize o arquivo `EBOOT.PBP`:**
    No Windows Explorer, você pode acessar o sistema de arquivos do WSL digitando `\wsl$` na barra de endereços. Navegue até o seu diretório Ubuntu e, em seguida, para o diretório do seu projeto:
    `\wsl$\Ubuntu\home\<seu_usuario_wsl>\psp-project\`
    Dentro deste diretório, você encontrará o arquivo `EBOOT.PBP`.

2.  **Abra com PPSSPP:**
    Inicie o emulador PPSSPP no Windows. Use a opção "File" -> "Load..." (ou equivalente) e navegue até o caminho `\wsl$\Ubuntu\home\<seu_usuario_wsl>\psp-project\EBOOT.PBP` para carregar e executar seu jogo.

## 5. Transferência para PSP Físico (Opcional)

Para transferir o jogo para um PSP físico:

1.  **Conecte seu PSP ao PC:** Conecte seu PSP ao computador via cabo USB e coloque-o no modo USB (geralmente em "Configurações" -> "Conexão USB").

2.  **Crie a estrutura de pastas:** No cartão de memória do seu PSP (geralmente `ms0:/`), navegue até `PSP/GAME/`. Crie uma nova pasta com o nome do seu jogo (ex: `ms0:/PSP/GAME/HelloWorld/`).

3.  **Copie o `EBOOT.PBP`:** Copie o arquivo `EBOOT.PBP` (localizado em `\wsl$\Ubuntu\home\<seu_usuario_wsl>\psp-project\`) para a pasta que você acabou de criar no seu PSP (ex: `ms0:/PSP/GAME/HelloWorld/EBOOT.PBP`).

4.  **Desconecte e jogue:** Desconecte seu PSP do PC. O jogo deverá aparecer na seção "Jogos" do seu PSP.
