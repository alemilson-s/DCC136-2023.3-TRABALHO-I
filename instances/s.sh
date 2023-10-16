diretorio="/home/alemilson/Documentos/DCC136-2023.3-TRABALHO-I/instances"

# Nome do arquivo de saída
diretorio_saida="/home/alemilson/Documentos/DCC136-2023.3-TRABALHO-I/instances"

if [ ! -d "$diretorio_saida" ]; then
    mkdir -p "$diretorio_saida"
fi

# Loop para criar arquivos de texto vazios com nomes terminados em "time"
for arquivo in "$diretorio"/*; do
    if [ -f "$arquivo" ]; then
        nome_arquivo_saida="$diretorio_saida/$(basename "$arquivo")_time.txt"
        touch "$nome_arquivo_saida"
    fi
done

echo "Arquivos de texto vazios com nomes terminados em 'time' foram criados em $diretorio_saida"
