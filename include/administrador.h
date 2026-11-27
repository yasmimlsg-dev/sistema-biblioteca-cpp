#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H

#include <string>
#include <vector>

/**
 * @brief Estrutura que representa os dados persistidos de um administrador.
 */
struct DadosAdministrador {
    int id = 0;
    std::string nome;
    std::string usuario;
    std::string senha;
    bool ativo = true;
};

/**
 * @brief Entidade Administrador do sistema da biblioteca.
 */
class Administrador {
private:
    DadosAdministrador dados_;

public:
    Administrador() = default;
    explicit Administrador(const DadosAdministrador& dados);
    Administrador(int id,
                  const std::string& nome,
                  const std::string& usuario,
                  const std::string& senha,
                  bool ativo = true);

    int getId() const;
    const std::string& getNome() const;
    const std::string& getUsuario() const;
    const std::string& getSenha() const;
    bool isAtivo() const;
    const DadosAdministrador& getDados() const;

    void setId(int id);
    void setNome(const std::string& nome);
    void setUsuario(const std::string& usuario);
    void setSenha(const std::string& senha);
    void setAtivo(bool ativo);

    /**
     * @brief Verifica se o administrador esta ativo e se usuario e senha conferem.
     */
    bool verificarCredenciais(const std::string& usuario, const std::string& senha) const;

    /**
     * @brief Serializa o registro no formato texto "id;nome;usuario;senha;ativo".
     */
    std::string serializar() const;

    /**
     * @brief Reconstroi um Administrador a partir de uma linha de texto.
     */
    static bool deserializar(const std::string& linha, Administrador& outAdmin);
};

/**
 * @brief Gerencia autenticacao, sessao, CRUD e persistencia de administradores.
 *
 * Mantem a colecao em memoria sempre ordenada por ID crescente para permitir
 * consultas por chave primaria via busca binaria.
 */
class GerenciadorAdministradores {
private:
    std::vector<Administrador> administradores_;
    std::string caminhoArquivo_;
    bool sessaoAtiva_ = false;
    int idAdministradorLogado_ = -1;

    void ordenarPorId();
    int buscarIndicePorId(int id) const;
    int buscarIndicePorUsuario(const std::string& usuario, bool ignorarCase = false) const;
    int contarAtivos() const;

    static std::string normalizarMinusculas(const std::string& texto);
    static bool contemCaracteresInvalidos(const std::string& texto);

public:
    explicit GerenciadorAdministradores(
        const std::string& caminhoArquivo = "dados/administradores.txt");

    // Persistencia em arquivo texto (.txt)
    bool carregarArquivo(bool& criouPadrao);
    bool salvarArquivo() const;

    // Acesso administrativo e controle de sessao
    bool autenticar(const std::string& usuario, const std::string& senha);
    void encerrarSessao();
    bool estaAutenticado() const;
    bool exigirAutenticacao() const;
    const Administrador* getAdministradorLogado() const;

    // CRUD e gerenciamento de credenciais
    int gerarProximoId() const;
    bool cadastrar(const std::string& nome,
                   const std::string& usuario,
                   const std::string& senha,
                   std::string& mensagemErro);
    const std::vector<Administrador>& listar() const;
    const Administrador* consultarPorId(int id) const;
    const Administrador* consultarPorUsuario(const std::string& usuario) const;
    bool alterarDados(int id,
                      const std::string& novoNome,
                      bool novoAtivo,
                      std::string& mensagemErro);
    bool alterarCredenciais(int id,
                            const std::string& novoUsuario,
                            const std::string& novaSenha,
                            std::string& mensagemErro);
    bool excluir(int id, std::string& mensagemErro);
};

#endif // ADMINISTRADOR_H
