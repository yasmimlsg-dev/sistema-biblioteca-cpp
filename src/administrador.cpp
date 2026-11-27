#include "../include/administrador.h"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace {

std::string removerEspacosExtremos(const std::string& texto) {
    std::size_t inicio = 0;
    while (inicio < texto.size() &&
           std::isspace(static_cast<unsigned char>(texto[inicio]))) {
        ++inicio;
    }

    std::size_t fim = texto.size();
    while (fim > inicio &&
           std::isspace(static_cast<unsigned char>(texto[fim - 1]))) {
        --fim;
    }

    return texto.substr(inicio, fim - inicio);
}

bool contemEspaco(const std::string& texto) {
    for (char ch : texto) {
        if (std::isspace(static_cast<unsigned char>(ch))) {
            return true;
        }
    }
    return false;
}

bool validarNome(const std::string& nome, std::string& nomeLimpo, std::string& mensagemErro) {
    nomeLimpo = removerEspacosExtremos(nome);
    if (nomeLimpo.empty()) {
        mensagemErro = "O nome do administrador nao pode ser vazio.";
        return false;
    }
    for (char ch : nomeLimpo) {
        if (ch == ';' || ch == '\n' || ch == '\r') {
            mensagemErro = "O nome nao pode conter ';' ou quebras de linha.";
            return false;
        }
    }
    return true;
}

bool validarUsuario(const std::string& usuario, std::string& mensagemErro) {
    if (usuario.empty()) {
        mensagemErro = "O nome de usuario nao pode ser vazio.";
        return false;
    }
    if (contemEspaco(usuario)) {
        mensagemErro = "O nome de usuario nao pode conter espacos.";
        return false;
    }
    for (char ch : usuario) {
        if (ch == ';' || ch == '\n' || ch == '\r') {
            mensagemErro = "O nome de usuario nao pode conter ';' ou quebras de linha.";
            return false;
        }
    }
    return true;
}

bool validarSenha(const std::string& senha, std::string& mensagemErro) {
    if (senha.size() < 4) {
        mensagemErro = "A senha deve ter pelo menos 4 caracteres.";
        return false;
    }
    for (char ch : senha) {
        if (ch == ';' || ch == '\n' || ch == '\r') {
            mensagemErro = "A senha nao pode conter ';' ou quebras de linha.";
            return false;
        }
    }
    return true;
}

} // namespace

// ============================================================================
// Implementacao da classe Administrador
// ============================================================================

Administrador::Administrador(const DadosAdministrador& dados) : dados_(dados) {}

Administrador::Administrador(int id,
                             const std::string& nome,
                             const std::string& usuario,
                             const std::string& senha,
                             bool ativo)
    : dados_{id, nome, usuario, senha, ativo} {}

int Administrador::getId() const {
    return dados_.id;
}

const std::string& Administrador::getNome() const {
    return dados_.nome;
}

const std::string& Administrador::getUsuario() const {
    return dados_.usuario;
}

const std::string& Administrador::getSenha() const {
    return dados_.senha;
}

bool Administrador::isAtivo() const {
    return dados_.ativo;
}

const DadosAdministrador& Administrador::getDados() const {
    return dados_;
}

void Administrador::setId(int id) {
    dados_.id = id;
}

void Administrador::setNome(const std::string& nome) {
    dados_.nome = nome;
}

void Administrador::setUsuario(const std::string& usuario) {
    dados_.usuario = usuario;
}

void Administrador::setSenha(const std::string& senha) {
    dados_.senha = senha;
}

void Administrador::setAtivo(bool ativo) {
    dados_.ativo = ativo;
}

bool Administrador::verificarCredenciais(const std::string& usuario,
                                         const std::string& senha) const {
    return dados_.ativo && dados_.usuario == usuario && dados_.senha == senha;
}

std::string Administrador::serializar() const {
    std::ostringstream oss;
    oss << dados_.id << ';'
        << dados_.nome << ';'
        << dados_.usuario << ';'
        << dados_.senha << ';'
        << (dados_.ativo ? '1' : '0');
    return oss.str();
}

bool Administrador::deserializar(const std::string& linha, Administrador& outAdmin) {
    std::string limpa = linha;
    if (!limpa.empty() && limpa.back() == '\r') {
        limpa.pop_back();
    }

    std::vector<std::string> campos;
    std::size_t inicio = 0;
    while (true) {
        std::size_t pos = limpa.find(';', inicio);
        if (pos == std::string::npos) {
            campos.push_back(limpa.substr(inicio));
            break;
        }
        campos.push_back(limpa.substr(inicio, pos - inicio));
        inicio = pos + 1;
    }

    if (campos.size() != 5) {
        return false;
    }

    int id = 0;
    try {
        std::size_t processados = 0;
        id = std::stoi(campos[0], &processados);
        if (processados != campos[0].size() || id <= 0) {
            return false;
        }
    } catch (...) {
        return false;
    }

    const std::string& nome = campos[1];
    const std::string& usuario = campos[2];
    const std::string& senha = campos[3];
    const std::string& ativoStr = campos[4];

    if (nome.empty() || usuario.empty() || senha.empty()) {
        return false;
    }

    bool ativo = false;
    if (ativoStr == "1") {
        ativo = true;
    } else if (ativoStr == "0") {
        ativo = false;
    } else {
        return false;
    }

    outAdmin = Administrador(id, nome, usuario, senha, ativo);
    return true;
}

// ============================================================================
// Implementacao da classe GerenciadorAdministradores
// ============================================================================

GerenciadorAdministradores::GerenciadorAdministradores(const std::string& caminhoArquivo)
    : caminhoArquivo_(caminhoArquivo) {}

void GerenciadorAdministradores::ordenarPorId() {
    // Insertion Sort para manter a lista ordenada pela chave primaria (id).
    for (std::size_t i = 1; i < administradores_.size(); ++i) {
        Administrador chave = administradores_[i];
        std::size_t j = i;
        while (j > 0 && administradores_[j - 1].getId() > chave.getId()) {
            administradores_[j] = administradores_[j - 1];
            --j;
        }
        administradores_[j] = chave;
    }
}

int GerenciadorAdministradores::buscarIndicePorId(int id) const {
    // Busca binaria sobre a colecao ordenada por id.
    int esquerda = 0;
    int direita = static_cast<int>(administradores_.size()) - 1;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        int idMeio = administradores_[static_cast<std::size_t>(meio)].getId();
        if (idMeio == id) {
            return meio;
        }
        if (idMeio < id) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    return -1;
}

int GerenciadorAdministradores::buscarIndicePorUsuario(const std::string& usuario,
                                                       bool ignorarCase) const {
    const std::string alvo = ignorarCase ? normalizarMinusculas(usuario) : usuario;
    for (std::size_t i = 0; i < administradores_.size(); ++i) {
        const std::string& atual = administradores_[i].getUsuario();
        if (ignorarCase) {
            if (normalizarMinusculas(atual) == alvo) {
                return static_cast<int>(i);
            }
        } else if (atual == alvo) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int GerenciadorAdministradores::contarAtivos() const {
    int total = 0;
    for (const auto& admin : administradores_) {
        if (admin.isAtivo()) {
            ++total;
        }
    }
    return total;
}

std::string GerenciadorAdministradores::normalizarMinusculas(const std::string& texto) {
    std::string resultado = texto;
    for (char& ch : resultado) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return resultado;
}

bool GerenciadorAdministradores::contemCaracteresInvalidos(const std::string& texto) {
    for (char ch : texto) {
        if (ch == ';' || ch == '\n' || ch == '\r') {
            return true;
        }
    }
    return false;
}

bool GerenciadorAdministradores::carregarArquivo(bool& criouPadrao) {
    criouPadrao = false;
    administradores_.clear();
    sessaoAtiva_ = false;
    idAdministradorLogado_ = -1;

    std::ifstream arquivo(caminhoArquivo_);
    if (arquivo.is_open()) {
        std::string linha;
        while (std::getline(arquivo, linha)) {
            std::string semEspacos = removerEspacosExtremos(linha);
            if (semEspacos.empty() || semEspacos[0] == '#') {
                continue;
            }
            Administrador admin;
            if (Administrador::deserializar(linha, admin)) {
                administradores_.push_back(admin);
            }
        }
        arquivo.close();
    }

    if (administradores_.empty()) {
        administradores_.emplace_back(1, "Administrador Padrao", "admin", "admin123", true);
        criouPadrao = true;
        return salvarArquivo();
    }

    ordenarPorId();
    return true;
}

bool GerenciadorAdministradores::salvarArquivo() const {
    std::error_code ec;
    std::filesystem::path caminho(caminhoArquivo_);
    if (caminho.has_parent_path()) {
        std::filesystem::create_directories(caminho.parent_path(), ec);
        if (ec) {
            return false;
        }
    }

    std::ofstream arquivo(caminhoArquivo_, std::ios::trunc);
    if (!arquivo.is_open()) {
        return false;
    }

    for (const auto& admin : administradores_) {
        arquivo << admin.serializar() << '\n';
    }
    arquivo.flush();
    return arquivo.good();
}

bool GerenciadorAdministradores::autenticar(const std::string& usuario,
                                            const std::string& senha) {
    for (const auto& admin : administradores_) {
        if (admin.verificarCredenciais(usuario, senha)) {
            sessaoAtiva_ = true;
            idAdministradorLogado_ = admin.getId();
            return true;
        }
    }
    return false;
}

void GerenciadorAdministradores::encerrarSessao() {
    sessaoAtiva_ = false;
    idAdministradorLogado_ = -1;
}

bool GerenciadorAdministradores::estaAutenticado() const {
    if (!sessaoAtiva_) {
        return false;
    }
    int indice = buscarIndicePorId(idAdministradorLogado_);
    return indice != -1 && administradores_[static_cast<std::size_t>(indice)].isAtivo();
}

bool GerenciadorAdministradores::exigirAutenticacao() const {
    return estaAutenticado();
}

const Administrador* GerenciadorAdministradores::getAdministradorLogado() const {
    if (!estaAutenticado()) {
        return nullptr;
    }
    int indice = buscarIndicePorId(idAdministradorLogado_);
    if (indice == -1) {
        return nullptr;
    }
    return &administradores_[static_cast<std::size_t>(indice)];
}

int GerenciadorAdministradores::gerarProximoId() const {
    if (administradores_.empty()) {
        return 1;
    }
    return administradores_.back().getId() + 1;
}

bool GerenciadorAdministradores::cadastrar(const std::string& nome,
                                           const std::string& usuario,
                                           const std::string& senha,
                                           std::string& mensagemErro) {
    std::string nomeLimpo;
    if (!validarNome(nome, nomeLimpo, mensagemErro)) {
        return false;
    }
    if (!validarUsuario(usuario, mensagemErro)) {
        return false;
    }
    if (!validarSenha(senha, mensagemErro)) {
        return false;
    }
    if (contemCaracteresInvalidos(nomeLimpo) ||
        contemCaracteresInvalidos(usuario) ||
        contemCaracteresInvalidos(senha)) {
        mensagemErro = "Os campos nao podem conter ';' ou quebras de linha.";
        return false;
    }

    if (buscarIndicePorUsuario(usuario, true) != -1) {
        mensagemErro = "Ja existe um administrador cadastrado com este usuario.";
        return false;
    }

    int novoId = gerarProximoId();
    administradores_.emplace_back(novoId, nomeLimpo, usuario, senha, true);
    ordenarPorId();

    if (!salvarArquivo()) {
        int indice = buscarIndicePorId(novoId);
        if (indice != -1) {
            administradores_.erase(administradores_.begin() + indice);
        }
        mensagemErro = "Nao foi possivel gravar o arquivo de administradores.";
        return false;
    }

    mensagemErro.clear();
    return true;
}

const std::vector<Administrador>& GerenciadorAdministradores::listar() const {
    return administradores_;
}

const Administrador* GerenciadorAdministradores::consultarPorId(int id) const {
    int indice = buscarIndicePorId(id);
    if (indice == -1) {
        return nullptr;
    }
    return &administradores_[static_cast<std::size_t>(indice)];
}

const Administrador* GerenciadorAdministradores::consultarPorUsuario(
    const std::string& usuario) const {
    int indice = buscarIndicePorUsuario(usuario, false);
    if (indice == -1) {
        return nullptr;
    }
    return &administradores_[static_cast<std::size_t>(indice)];
}

bool GerenciadorAdministradores::alterarDados(int id,
                                              const std::string& novoNome,
                                              bool novoAtivo,
                                              std::string& mensagemErro) {
    std::string nomeLimpo;
    if (!validarNome(novoNome, nomeLimpo, mensagemErro)) {
        return false;
    }

    int indice = buscarIndicePorId(id);
    if (indice == -1) {
        mensagemErro = "Administrador nao encontrado.";
        return false;
    }

    Administrador& admin = administradores_[static_cast<std::size_t>(indice)];
    if (admin.isAtivo() && !novoAtivo) {
        if (sessaoAtiva_ && id == idAdministradorLogado_) {
            mensagemErro = "Nao e permitido desativar o administrador atualmente autenticado.";
            return false;
        }
        if (contarAtivos() <= 1) {
            mensagemErro = "Nao e permitido desativar o ultimo administrador ativo do sistema.";
            return false;
        }
    }

    DadosAdministrador anterior = admin.getDados();
    admin.setNome(nomeLimpo);
    admin.setAtivo(novoAtivo);

    if (!salvarArquivo()) {
        admin = Administrador(anterior);
        mensagemErro = "Nao foi possivel salvar as alteracoes no arquivo.";
        return false;
    }

    mensagemErro.clear();
    return true;
}

bool GerenciadorAdministradores::alterarCredenciais(int id,
                                                    const std::string& novoUsuario,
                                                    const std::string& novaSenha,
                                                    std::string& mensagemErro) {
    if (!validarUsuario(novoUsuario, mensagemErro)) {
        return false;
    }
    if (!validarSenha(novaSenha, mensagemErro)) {
        return false;
    }

    int indice = buscarIndicePorId(id);
    if (indice == -1) {
        mensagemErro = "Administrador nao encontrado.";
        return false;
    }

    int indiceExistente = buscarIndicePorUsuario(novoUsuario, true);
    if (indiceExistente != -1 &&
        administradores_[static_cast<std::size_t>(indiceExistente)].getId() != id) {
        mensagemErro = "Ja existe outro administrador utilizando este nome de usuario.";
        return false;
    }

    Administrador& admin = administradores_[static_cast<std::size_t>(indice)];
    DadosAdministrador anterior = admin.getDados();
    admin.setUsuario(novoUsuario);
    admin.setSenha(novaSenha);

    if (!salvarArquivo()) {
        admin = Administrador(anterior);
        mensagemErro = "Nao foi possivel salvar as credenciais no arquivo.";
        return false;
    }

    mensagemErro.clear();
    return true;
}

bool GerenciadorAdministradores::excluir(int id, std::string& mensagemErro) {
    int indice = buscarIndicePorId(id);
    if (indice == -1) {
        mensagemErro = "Administrador nao encontrado.";
        return false;
    }

    if (sessaoAtiva_ && id == idAdministradorLogado_) {
        mensagemErro = "Nao e permitido excluir o administrador atualmente autenticado.";
        return false;
    }

    const Administrador& alvo = administradores_[static_cast<std::size_t>(indice)];
    if (alvo.isAtivo() && contarAtivos() <= 1) {
        mensagemErro = "Nao e permitido excluir o ultimo administrador ativo do sistema.";
        return false;
    }

    Administrador backup = alvo;
    administradores_.erase(administradores_.begin() + indice);

    if (!salvarArquivo()) {
        administradores_.push_back(backup);
        ordenarPorId();
        mensagemErro = "Nao foi possivel atualizar o arquivo apos a exclusao.";
        return false;
    }

    mensagemErro.clear();
    return true;
}
