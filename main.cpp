#include "include/administrador.h"

#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

constexpr int MAX_TENTATIVAS_LOGIN = 3;

void configurarConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

bool lerLinha(const std::string& prompt, std::string& destino) {
    std::cout << prompt;
    if (!std::getline(std::cin, destino)) {
        return false;
    }
    if (!destino.empty() && destino.back() == '\r') {
        destino.pop_back();
    }
    return true;
}

bool lerInteiro(const std::string& prompt, int& destino) {
    while (true) {
        std::string entrada;
        if (!lerLinha(prompt, entrada)) {
            return false;
        }
        try {
            std::size_t processados = 0;
            int valor = std::stoi(entrada, &processados);
            while (processados < entrada.size() &&
                   std::isspace(static_cast<unsigned char>(entrada[processados]))) {
                ++processados;
            }
            if (processados == entrada.size()) {
                destino = valor;
                return true;
            }
        } catch (...) {
        }
        std::cout << "Entrada invalida. Informe um numero inteiro.\n";
    }
}

bool lerConfirmacao(const std::string& prompt, bool& confirmado) {
    while (true) {
        std::string entrada;
        if (!lerLinha(prompt, entrada)) {
            return false;
        }
        if (entrada == "s" || entrada == "S" || entrada == "sim" || entrada == "SIM") {
            confirmado = true;
            return true;
        }
        if (entrada == "n" || entrada == "N" || entrada == "nao" || entrada == "NAO") {
            confirmado = false;
            return true;
        }
        std::cout << "Opcao invalida. Digite 's' para sim ou 'n' para nao.\n";
    }
}

void exibirLinhaDivisoria() {
    std::cout << "============================================================\n";
}

void exibirAdministrador(const Administrador& admin) {
    std::cout << "ID: " << admin.getId()
              << " | Nome: " << admin.getNome()
              << " | Usuario: " << admin.getUsuario()
              << " | Senha: ********"
              << " | Status: " << (admin.isAtivo() ? "Ativo" : "Inativo")
              << '\n';
}

bool verificarAcesso(const GerenciadorAdministradores& gerenciadorAdmin) {
    if (!gerenciadorAdmin.exigirAutenticacao()) {
        std::cout << "\nAcesso negado. Faca login como administrador.\n";
        return false;
    }
    return true;
}

bool realizarLogin(GerenciadorAdministradores& gerenciadorAdmin) {
    std::cout << '\n';
    exibirLinhaDivisoria();
    std::cout << "                    LOGIN ADMINISTRATIVO                    \n";
    exibirLinhaDivisoria();

    for (int tentativa = 1; tentativa <= MAX_TENTATIVAS_LOGIN; ++tentativa) {
        std::string usuario;
        std::string senha;

        if (!lerLinha("Usuario: ", usuario) || !lerLinha("Senha: ", senha)) {
            return false;
        }

        if (gerenciadorAdmin.autenticar(usuario, senha)) {
            const Administrador* logado = gerenciadorAdmin.getAdministradorLogado();
            std::cout << "\nAutenticacao realizada com sucesso! Bem-vindo(a), "
                      << (logado ? logado->getNome() : usuario) << ".\n";
            return true;
        }

        std::cout << "Credenciais invalidas ou administrador inativo. "
                  << "Tentativa " << tentativa << " de " << MAX_TENTATIVAS_LOGIN << ".\n";
    }

    std::cout << "Numero maximo de tentativas excedido.\n";
    return true;
}

// ============================================================================
// Pontos de integracao dos modulos da equipe
// ============================================================================

void menuLivros(const GerenciadorAdministradores& gerenciadorAdmin) {
    if (!verificarAcesso(gerenciadorAdmin)) {
        return;
    }
    // Integracao Pessoa 2: incluir include/livro.h e chamar o fluxo de livros.
    std::cout << "\n[Modulo de Gerenciamento de Livros]\n"
              << "Ponto de integracao reservado para o modulo da Pessoa 2 (include/livro.h).\n";
}

void menuClientes(const GerenciadorAdministradores& gerenciadorAdmin) {
    if (!verificarAcesso(gerenciadorAdmin)) {
        return;
    }
    // Integracao Pessoa 3: incluir include/cliente.h e chamar o fluxo de clientes.
    std::cout << "\n[Modulo de Gerenciamento de Clientes]\n"
              << "Ponto de integracao reservado para o modulo da Pessoa 3 (include/cliente.h).\n";
}

void menuEmprestimos(const GerenciadorAdministradores& gerenciadorAdmin) {
    if (!verificarAcesso(gerenciadorAdmin)) {
        return;
    }
    // Integracao Pessoa 4: incluir include/emprestimo.h e chamar o fluxo de emprestimos.
    std::cout << "\n[Modulo de Gerenciamento de Emprestimos e Devolucoes]\n"
              << "Ponto de integracao reservado para o modulo da Pessoa 4 (include/emprestimo.h).\n";
}

void menuConsultasRelatorios(const GerenciadorAdministradores& gerenciadorAdmin) {
    if (!verificarAcesso(gerenciadorAdmin)) {
        return;
    }
    // Integracao Pessoa 5: incluir include/ordenacao.h, include/busca.h, include/arquivos.h.
    std::cout << "\n[Modulo de Consultas e Relatorios]\n"
              << "Ponto de integracao reservado para o modulo da Pessoa 5 "
              << "(ordenacao.h, busca.h, arquivos.h).\n";
}

// ============================================================================
// Submenu de Gerenciamento de Administradores (Pessoa 1)
// ============================================================================

bool cadastrarAdministradorUI(GerenciadorAdministradores& gerenciadorAdmin) {
    std::string nome;
    std::string usuario;
    std::string senha;

    std::cout << "\n--- Cadastrar Administrador ---\n";
    if (!lerLinha("Nome completo: ", nome) ||
        !lerLinha("Usuario (sem espacos): ", usuario) ||
        !lerLinha("Senha (minimo 4 caracteres): ", senha)) {
        return false;
    }

    std::string erro;
    if (gerenciadorAdmin.cadastrar(nome, usuario, senha, erro)) {
        std::cout << "Administrador cadastrado com sucesso!\n";
    } else {
        std::cout << "Erro ao cadastrar: " << erro << '\n';
    }
    return true;
}

void listarAdministradoresUI(const GerenciadorAdministradores& gerenciadorAdmin) {
    std::cout << "\n--- Lista de Administradores ---\n";
    const auto& lista = gerenciadorAdmin.listar();
    if (lista.empty()) {
        std::cout << "Nenhum administrador cadastrado.\n";
        return;
    }
    for (const auto& admin : lista) {
        exibirAdministrador(admin);
    }
}

bool consultarAdministradorPorIdUI(const GerenciadorAdministradores& gerenciadorAdmin) {
    int id = 0;
    std::cout << "\n--- Consultar Administrador por ID (Busca Binaria) ---\n";
    if (!lerInteiro("Informe o ID: ", id)) {
        return false;
    }

    const Administrador* encontrado = gerenciadorAdmin.consultarPorId(id);
    if (encontrado == nullptr) {
        std::cout << "Administrador com ID " << id << " nao encontrado.\n";
    } else {
        exibirAdministrador(*encontrado);
    }
    return true;
}

bool consultarAdministradorPorUsuarioUI(const GerenciadorAdministradores& gerenciadorAdmin) {
    std::string usuario;
    std::cout << "\n--- Consultar Administrador por Usuario ---\n";
    if (!lerLinha("Informe o usuario: ", usuario)) {
        return false;
    }

    const Administrador* encontrado = gerenciadorAdmin.consultarPorUsuario(usuario);
    if (encontrado == nullptr) {
        std::cout << "Administrador com usuario '" << usuario << "' nao encontrado.\n";
    } else {
        exibirAdministrador(*encontrado);
    }
    return true;
}

bool alterarDadosAdministradorUI(GerenciadorAdministradores& gerenciadorAdmin) {
    int id = 0;
    std::cout << "\n--- Alterar Dados do Administrador ---\n";
    if (!lerInteiro("ID do administrador: ", id)) {
        return false;
    }

    const Administrador* atual = gerenciadorAdmin.consultarPorId(id);
    if (atual == nullptr) {
        std::cout << "Administrador com ID " << id << " nao encontrado.\n";
        return true;
    }

    std::cout << "Registro atual -> ";
    exibirAdministrador(*atual);

    std::string novoNome;
    if (!lerLinha("Novo nome: ", novoNome)) {
        return false;
    }

    bool novoAtivo = true;
    if (!lerConfirmacao("Manter administrador ativo? (s/n): ", novoAtivo)) {
        return false;
    }

    std::string erro;
    if (gerenciadorAdmin.alterarDados(id, novoNome, novoAtivo, erro)) {
        std::cout << "Dados atualizados com sucesso!\n";
    } else {
        std::cout << "Erro ao atualizar dados: " << erro << '\n';
    }
    return true;
}

bool alterarCredenciaisAdministradorUI(GerenciadorAdministradores& gerenciadorAdmin) {
    int id = 0;
    std::cout << "\n--- Alterar Credenciais do Administrador ---\n";
    if (!lerInteiro("ID do administrador: ", id)) {
        return false;
    }

    const Administrador* atual = gerenciadorAdmin.consultarPorId(id);
    if (atual == nullptr) {
        std::cout << "Administrador com ID " << id << " nao encontrado.\n";
        return true;
    }

    std::string novoUsuario;
    std::string novaSenha;
    if (!lerLinha("Novo usuario (sem espacos): ", novoUsuario) ||
        !lerLinha("Nova senha (minimo 4 caracteres): ", novaSenha)) {
        return false;
    }

    std::string erro;
    if (gerenciadorAdmin.alterarCredenciais(id, novoUsuario, novaSenha, erro)) {
        std::cout << "Credenciais atualizadas com sucesso!\n";
    } else {
        std::cout << "Erro ao atualizar credenciais: " << erro << '\n';
    }
    return true;
}

bool excluirAdministradorUI(GerenciadorAdministradores& gerenciadorAdmin) {
    int id = 0;
    std::cout << "\n--- Excluir Administrador ---\n";
    if (!lerInteiro("ID do administrador a excluir: ", id)) {
        return false;
    }

    const Administrador* atual = gerenciadorAdmin.consultarPorId(id);
    if (atual == nullptr) {
        std::cout << "Administrador com ID " << id << " nao encontrado.\n";
        return true;
    }

    std::cout << "Registro selecionado -> ";
    exibirAdministrador(*atual);

    bool confirmado = false;
    if (!lerConfirmacao("Confirma a exclusao? (s/n): ", confirmado)) {
        return false;
    }
    if (!confirmado) {
        std::cout << "Exclusao cancelada.\n";
        return true;
    }

    std::string erro;
    if (gerenciadorAdmin.excluir(id, erro)) {
        std::cout << "Administrador excluido com sucesso!\n";
    } else {
        std::cout << "Erro ao excluir: " << erro << '\n';
    }
    return true;
}

bool menuAdministradores(GerenciadorAdministradores& gerenciadorAdmin) {
    if (!verificarAcesso(gerenciadorAdmin)) {
        return true;
    }

    while (gerenciadorAdmin.exigirAutenticacao()) {
        std::cout << '\n';
        exibirLinhaDivisoria();
        std::cout << "              GERENCIAMENTO DE ADMINISTRADORES              \n";
        exibirLinhaDivisoria();
        std::cout << "1. Cadastrar administrador\n"
                  << "2. Listar administradores\n"
                  << "3. Consultar administrador por ID\n"
                  << "4. Consultar administrador por usuario\n"
                  << "5. Alterar dados / status\n"
                  << "6. Alterar credenciais (usuario e senha)\n"
                  << "7. Excluir administrador\n"
                  << "0. Voltar ao menu principal\n";

        int opcao = -1;
        if (!lerInteiro("Escolha uma opcao: ", opcao)) {
            return false;
        }

        switch (opcao) {
            case 1:
                if (!cadastrarAdministradorUI(gerenciadorAdmin)) {
                    return false;
                }
                break;
            case 2:
                listarAdministradoresUI(gerenciadorAdmin);
                break;
            case 3:
                if (!consultarAdministradorPorIdUI(gerenciadorAdmin)) {
                    return false;
                }
                break;
            case 4:
                if (!consultarAdministradorPorUsuarioUI(gerenciadorAdmin)) {
                    return false;
                }
                break;
            case 5:
                if (!alterarDadosAdministradorUI(gerenciadorAdmin)) {
                    return false;
                }
                break;
            case 6:
                if (!alterarCredenciaisAdministradorUI(gerenciadorAdmin)) {
                    return false;
                }
                break;
            case 7:
                if (!excluirAdministradorUI(gerenciadorAdmin)) {
                    return false;
                }
                break;
            case 0:
                return true;
            default:
                std::cout << "Opcao invalida. Tente novamente.\n";
                break;
        }
    }
    return true;
}

} // namespace

int main() {
    configurarConsole();

    GerenciadorAdministradores gerenciadorAdmin("dados/administradores.txt");
    bool criouPadrao = false;
    if (!gerenciadorAdmin.carregarArquivo(criouPadrao)) {
        std::cerr << "Aviso: nao foi possivel inicializar o arquivo dados/administradores.txt.\n";
    } else if (criouPadrao) {
        std::cout << "[Info] Administrador padrao inicializado (usuario: admin | senha: admin123).\n";
    }

    bool executando = true;
    while (executando) {
        if (!gerenciadorAdmin.estaAutenticado()) {
            std::cout << '\n';
            exibirLinhaDivisoria();
            std::cout << "            SISTEMA DE GERENCIAMENTO DE BIBLIOTECA          \n";
            exibirLinhaDivisoria();
            std::cout << "1. Entrar (Login administrativo)\n"
                      << "0. Sair\n";

            int opcao = -1;
            if (!lerInteiro("Escolha uma opcao: ", opcao)) {
                break;
            }

            switch (opcao) {
                case 1:
                    if (!realizarLogin(gerenciadorAdmin)) {
                        executando = false;
                    }
                    break;
                case 0:
                    executando = false;
                    break;
                default:
                    std::cout << "Opcao invalida. Tente novamente.\n";
                    break;
            }
            continue;
        }

        const Administrador* logado = gerenciadorAdmin.getAdministradorLogado();
        std::cout << '\n';
        exibirLinhaDivisoria();
        std::cout << "            SISTEMA DE GERENCIAMENTO DE BIBLIOTECA          \n";
        if (logado != nullptr) {
            std::cout << "Administrador: " << logado->getNome()
                      << " (@" << logado->getUsuario() << ")\n";
        }
        exibirLinhaDivisoria();
        std::cout << "1. Gerenciamento de Livros\n"
                  << "2. Gerenciamento de Clientes\n"
                  << "3. Gerenciamento de Emprestimos e Devolucoes\n"
                  << "4. Consultas e Relatorios\n"
                  << "5. Gerenciamento de Administradores\n"
                  << "6. Trocar administrador (Logout)\n"
                  << "0. Sair do sistema\n";

        int opcao = -1;
        if (!lerInteiro("Escolha uma opcao: ", opcao)) {
            break;
        }

        switch (opcao) {
            case 1:
                menuLivros(gerenciadorAdmin);
                break;
            case 2:
                menuClientes(gerenciadorAdmin);
                break;
            case 3:
                menuEmprestimos(gerenciadorAdmin);
                break;
            case 4:
                menuConsultasRelatorios(gerenciadorAdmin);
                break;
            case 5:
                if (!menuAdministradores(gerenciadorAdmin)) {
                    executando = false;
                }
                break;
            case 6:
                gerenciadorAdmin.encerrarSessao();
                std::cout << "Sessao encerrada com sucesso.\n";
                break;
            case 0:
                executando = false;
                break;
            default:
                std::cout << "Opcao invalida. Tente novamente.\n";
                break;
        }
    }

    std::cout << "\nEncerrando o Sistema de Gerenciamento de Biblioteca.\n";
    return 0;
}
