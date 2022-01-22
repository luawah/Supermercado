
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>

// Valores maximos para validacao e impressao
#define DESCRICAO_MAX 50
#define DESCRICAO_MIN 6
#define CODIGO_MAX 6
#define LINHA_MAX 90
#define DEFAULT_MAX 10

// Limite de produtos no estoque
#define PRODUTOS_MAX 50
struct Produto {
    int codigo;
    std::string descricao;
    char categoria;
    int quantidade;
    float preco;

    // Recebe da entrada os valores para o produto
    void ler() {
        codigo_ler();
        descricao_ler();
        categoria_ler();
        quantidade_ler();
        preco_ler();
    }
    // Mostrar dados na tela
    void imprimir() {

        // SETW determina a largura minima em caracteres do campo impresso
        // SETFIXED para exibir numeros fixos (sem notacao cientifica)
        // SETPRECISION determina a quantidade de casasa apos a virgula

        std::cout << std::setw(CODIGO_MAX) << codigo << ' ';
        std::cout << std::setw(DESCRICAO_MAX) << descricao << ' ';
        std::cout << std::setw(DEFAULT_MAX) << categoria << ' ';
        std::cout << std::setw(DEFAULT_MAX) << quantidade << ' ';
        std::cout << std::setw(DEFAULT_MAX) << std::fixed << std::setprecision(2) << preco << ' ';
    }
    // Retorna positivo se possuir maior prioridade (descricao/categoria) que outro_produto
    bool prioridade(Produto outro_produto) {
        bool prioridade_maior = false;
        if (descricao > outro_produto.descricao) {
            prioridade_maior = true;
        } else if (descricao == outro_produto.descricao) {
            if (categoria > outro_produto.categoria) {
                prioridade_maior = true;
            }
        }
        return prioridade_maior;
    }

    //Funoces de leitura e validacao

    int digitos_contar(int numero) {
        int digitos = 0;
        int temp = numero;
        while (temp > 0) {
            temp /= DEFAULT_MAX;
            ++digitos;
        }
        return digitos;
    }
    void codigo_ler() {
        std::cout << "Codigo: ";
        bool codigo_valido = false;
        while (!codigo_valido) {
            std::cin >> std::ws >> codigo;
            int digitos = digitos_contar(codigo);

            if (digitos < CODIGO_MAX || digitos > CODIGO_MAX) {
                std::cout << "Codigo invalido, informe novamente: ";
            } else {
                codigo_valido = true;
            }
        }
    }
    void descricao_ler() {
        std::cout << "Descricao: ";
        bool descricao_valida = false;
        while (!descricao_valida) {
            std::cin >> std::ws;
            std::getline(std::cin, descricao);
            if (descricao.length() < DESCRICAO_MIN || descricao.length() > DESCRICAO_MAX) {
                std::cout << "Descricao invalida, informe novamente: ";
            } else {
                descricao_valida = true;
            }
        }

        // Armazenar em caixa alta
        for (int i = 0; i < (int)descricao.length(); ++i) {
            descricao[i] = toupper(descricao[i]);
        }
    }
    void categoria_ler() {
        std::cout << "Categoria: ";
        std::cin >> std::ws >> categoria;
        categoria = toupper(categoria); // Caixa alta
    }
    void quantidade_ler() {
        std::cout << "Quantidade: ";
        bool quantidade_valida = false;
        while (!quantidade_valida) {
            std::cin >> std::ws >> quantidade;
            if (quantidade < 0) {
                std::cout << "Quantidade invalida, informe novamente: ";
            } else {
                quantidade_valida = true;
            }
        }
    }
    void preco_ler() {
        std::cout << "Preco: ";
        bool quantidade_valida = false;
        while (!quantidade_valida) {
            std::cin >> std::ws >> preco;
            if (preco < 0) {
                std::cout << "Preco invalido, informe novamente: ";
            } else {
                quantidade_valida = true;
            }
        }
    }
};

struct Estoque {
    Produto produtos[PRODUTOS_MAX];
    int total; // Quantidade de produtos inclusos no estoque.

    Estoque() {
        total = 0;
    }

    // Condicoes

    bool vazio() {
        return total == 0;
    }
    bool cheio() {
        return total == PRODUTOS_MAX;
    }
    bool contem(int codigo) {
        return buscar(codigo) > -1;
    }

    // Busca o indice do produto no estoque, retorna -1 se nao encontrar
    int buscar(int codigo) {
        int indice = -1;
        for (int i = 0; i < total; ++i) {
            if (produtos[i].codigo == codigo) {
                indice = i;
                break;
            }
        }
        return indice;
    }

    // Inclui em ordem de prioridade
    void incluir(Produto produto_incluido) {
        if (cheio()) {
            return;
        }

        // Efetua inclusao por ordem de maior prioridade (decricao/categoria)
        int indice = total;
        while (indice > 0 && produto_incluido.prioridade(produtos[indice])) {
            produtos[indice] = produtos[indice - 1];
            --indice;
        }
        produtos[indice] = produto_incluido;
        ++total;

        return;
    }

    // Excluir pelo codigo se houver quantidade = 0
    void excluir(int codigo) {
        int indice = buscar(codigo);
        if (indice < 0 || produtos[indice].quantidade > 0) {
            return;
        }

        // Exclui o produto.
        for (int j = indice; j < total; ++j) {
            produtos[j] = produtos[j + 1];
        }
        --total;

        return;
    }

    // Abate a quantidade informada do produto no estoque
    void abater(int codigo, int quantidade_abatida) {
        int indice = buscar(codigo);

        if (indice < 0 || produtos[indice].quantidade < quantidade_abatida) {
            return;
        }

        produtos[indice].quantidade -= quantidade_abatida;
    }

    // Lista produtos em estoque
    void listar() {
        if (vazio()) {
            std::cout << "Estoque vazio.\n";
            return;
        }

        imprimir_cabecalho();
        for (int i = 0; i < total; ++i) {
            produtos[i].imprimir();
            std::cout << '\n';
        }
        imprimir_linha();
    }

    // Lista o
    void listar_estoquebaixo(int quantidade_minima) {
        if (vazio()) {
            std::cout << "Estoque vazio.\n";
            return;
        }
        imprimir_cabecalho();
        for (int i = 0; i < total; ++i) {
            if (produtos[i].quantidade < quantidade_minima) {
                produtos[i].imprimir();
                std::cout << '\n';
            }
        }
        imprimir_linha();

        return;
    }

    void aumento(int codigo, int aumento_percentual) {
        int indice = buscar(codigo);
        if (indice < 0) {
            return;
        }
        // Aplica o aumento
        produtos[indice].preco += produtos[indice].preco * aumento_percentual / 100.0;
        return;
    }
    void desconto(int codigo, int desconto_percentual) {

        int indice = buscar(codigo);
        if (indice < 0) {
            return;
        }
        if (desconto_percentual < 0) {
            std::cout << "Percentual menor que zero!.\n";
            return;
        }
        if (desconto_percentual > 100) {
            std::cout << "Percentual maior que cem!\n";
            return;
        }

        // Aplica o desconto
        produtos[indice].preco -= produtos[indice].preco * desconto_percentual / 100.0;
        return;
    }

private:
    void imprimir_cabecalho() {
        imprimir_linha();

        // SETW determina a largura em caracteres do campo impresso

        std::cout << std::setw(CODIGO_MAX) << "Codigo" << ' ';
        std::cout << std::setw(DESCRICAO_MAX) << "Decricao" << ' ';
        std::cout << std::setw(DEFAULT_MAX) << "Categoria" << ' ';
        std::cout << std::setw(DEFAULT_MAX) << "Quantidade" << ' ';
        std::cout << std::setw(DEFAULT_MAX) << "Preco" << '\n';
        imprimir_linha();
    }

    void imprimir_linha() {
        for (int i = 0; i < LINHA_MAX; ++i) {
            std::cout << '-';
        }
        std::cout << '\n';
    }
};

struct Menu {

    // "Roda" o menu, processando a opcao informada ate encerrar
    void rodar(Estoque *estoque) {
        bool fim = false;

        while (!fim) {
            exibir();
            int opcao = numero_ler();
            switch (opcao) {
            case 1:
                opcao_incluir(estoque);
                break;
            case 2:
                opcao_excluir(estoque);
                break;
            case 3:
                opcao_abater(estoque);
                break;
            case 4:
                opcao_listagem(estoque);
                break;
            case 5:
                opcao_aumentooudesconto(estoque);
                break;
            case 6:
                std::cout << "Encerrando.\n";
                fim = true;
                break;
            default:
                std::cout << "Opcao invalida!\n";
                break;
            }
        }
    }

private:
    // Imprimir os menus na tela

    void exibir() {
        std::cout << "Menu Principal:\n";
        std::cout << "1-Incluir produto \n";
        std::cout << "2-Excluir produto\n";
        std::cout << "3-Abater do estoque\n";
        std::cout << "4-Listar produtos\n";
        std::cout << "5-Aplicar aumento/desconto\n";
        std::cout << "6-Fim\n";
    }
    void listagem_exibir() {
        std::cout << "Menu Listagem: \n";
        std::cout << "1-Listar todos os produtos (ordenados por categoria/descricao).\n";
        std::cout << "2-Listar produtos com estoque baixo.\n";
        std::cout << "3-Voltar\n";
    }

    // Ler um valor inteiro
    int numero_ler() {
        int opcao = 0;
        std::cin >> std::ws >> opcao;
        return opcao;
    }

    // Cada funcao realiza a operacao correspondente a uma opcao do menu principal

    void opcao_incluir(Estoque *estoque) {
        Produto produto = {};
        produto.ler();
        if (estoque->cheio()) {
            std::cout << "Estoque cheio!\n";
        } else {
            estoque->incluir(produto);
        }
    }
    void opcao_excluir(Estoque *estoque) {
        Produto produto = {};
        produto.codigo_ler();

        int indice = estoque->buscar(produto.codigo);
        if (indice > -1) {
            produto = estoque->produtos[indice];
            if (produto.quantidade > 0) {
                std::cout << "Produto \"" << produto.descricao << "\" tem " << produto.quantidade << " em estoque!\n";
            } else {
                estoque->excluir(produto.codigo);
                std::cout << "Produto \"" << produto.descricao << "\" excluido com sucesso!\n";
            }
        } else {
            std::cout << "Produto nao existe!\n";
        }
    }
    void opcao_abater(Estoque *estoque) {
        Produto produto = {};
        produto.codigo_ler();
        int indice = estoque->buscar(produto.codigo);
        int quantidade_abatida = 0;
        if (indice > -1) {
            produto = estoque->produtos[indice];
            std::cout << "Quantidade: ";
            quantidade_abatida = numero_ler();
            if (produto.quantidade < quantidade_abatida) {
                std::cout << "Produto \"" << produto.descricao << "\" tem somente " << produto.quantidade << " em estoque.\n";
            } else {
                estoque->abater(produto.codigo, quantidade_abatida);
                produto = estoque->produtos[indice];

                std::cout << "Produto \"" << produto.descricao << "\" agora tem " << produto.quantidade << " em estoque.\n";
            }
        } else {
            std::cout << "Produto nao existe!\n";
        }
    }
    void opcao_listagem(Estoque *estoque) {
        bool voltar = false;
        int quantidade_minima = 0;
        while (!voltar) {
            listagem_exibir();
            int opcao = numero_ler();
            switch (opcao) {
            case 1:
                estoque->listar();
                break;
            case 2:
                std::cout << "Qtd minima: ";
                quantidade_minima = numero_ler();
                estoque->listar_estoquebaixo(quantidade_minima);
                break;
            case 3:
                std::cout << "Voltando...\n";
                voltar = true;
                break;
            default:
                std::cout << "Opcao invalida!\n";
                break;
            }
        }
    }
    void opcao_aumentooudesconto(Estoque *estoque) {

        std::cout << "Operacao (A-aumento ou D-desconto): ";
        char operacao = 0;
        std::cin >> std::ws >> operacao;
        operacao = toupper(operacao);

        if (operacao != 'A' && operacao != 'D') {
            std::cout << "Operacao nao existe!\n";
            return;
        }

        Produto produto = {};
        produto.codigo_ler();

        if (estoque->contem(produto.codigo)) {

            std::cout << "Percentual: ";
            int percentual = numero_ler();
            if (percentual < 0) {
                std::cout << "Percentual menor que zero!\n";
                return;
            }

            if (operacao == 'A') {
                estoque->aumento(produto.codigo, percentual);
                std::cout << "Operacao realizada com sucesso. Produto aumentado!\n";
            } else {
                if (percentual > 100) {
                    std::cout << "Percentual maior que cem!\n";
                } else {
                    estoque->desconto(produto.codigo, percentual);
                    std::cout << "Operacao realizada com sucesso. Produto descontado!\n";
                }
            }
        } else {
            std::cout << "Produto nao existe!\n";
        }
    }
};

int main() {
    Estoque estoque = {};
    Menu menu = {};

    menu.rodar(&estoque);

    return 0;
}
