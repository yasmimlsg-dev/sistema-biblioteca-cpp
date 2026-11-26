# 📚 Sistema de Gerenciamento de Biblioteca

Sistema de gerenciamento de biblioteca desenvolvido em **C++**, como projeto interdisciplinar das disciplinas de **Estrutura de Dados** e **Programação Orientada a Objetos (POO)**.

O projeto tem como objetivo desenvolver uma aplicação capaz de organizar o acervo de livros, gerenciar clientes e administradores e controlar os processos de empréstimos e devoluções, aplicando conceitos fundamentais da programação e da organização de dados.

---

## 🎯 Objetivos do Projeto

Desenvolver um sistema de gerenciamento que permita administrar as principais atividades de uma biblioteca, utilizando estruturas de dados, algoritmos de ordenação, busca binária e manipulação de arquivos para armazenamento persistente.

## ⚙️ Funcionalidades

O sistema será dividido em módulos:

### 🔐 Acesso Administrativo
- Login com usuário e senha.
- Autenticação de administradores.
- Controle de acesso às funcionalidades administrativas.

### 📖 Gerenciamento de Livros
- Cadastro de livros.
- Consulta e listagem do acervo.
- Alteração e exclusão de registros.
- Controle de disponibilidade dos exemplares.

### 👥 Gerenciamento de Clientes
- Cadastro de leitores.
- Consulta e listagem de clientes.
- Alteração e exclusão de registros.

### 🛡️ Gerenciamento de Administradores
- Cadastro de administradores.
- Consulta e listagem.
- Alteração e exclusão de registros.
- Gerenciamento das credenciais de acesso.

### 🔄 Gerenciamento de Empréstimos
- Registro de empréstimos.
- Identificação do cliente e do livro emprestado.
- Controle das datas de empréstimo e devolução.
- Registro de devoluções.
- Atualização da disponibilidade dos livros.

### 📊 Consultas e Relatórios
- Consulta dos registros cadastrados.
- Histórico de empréstimos por cliente.
- Visualização de empréstimos ativos e finalizados.
- Quantidade de livros disponíveis e emprestados.

---

## 🧠 Conceitos Aplicados

O desenvolvimento do projeto busca aplicar os seguintes conceitos:

| Conceito | Aplicação |
|---|---|
| Programação Orientada a Objetos | Organização do sistema em classes, atributos e métodos. |
| Structs | Armazenamento e manipulação dos registros em memória. |
| CRUD | Inclusão, consulta, alteração e exclusão de registros. |
| Algoritmos de ordenação | Organização dos registros por chave primária. |
| Busca binária | Localização de registros em estruturas ordenadas. |
| Manipulação de arquivos | Leitura e gravação de dados em arquivos de texto. |

### 🔎 Algoritmos de Ordenação e Busca

Os registros serão organizados por suas respectivas chaves primárias, permitindo a aplicação de algoritmos de ordenação e busca binária.

Algoritmos previstos:
- Bubble Sort;
- Insertion Sort;
- Selection Sort;
- Quick Sort.

A equipe selecionará o algoritmo de ordenação a ser utilizado no sistema.

---

## 🛠️ Tecnologias Utilizadas

- **C++** — Linguagem de programação.
- **Git** — Controle de versão.
- **GitHub** — Hospedagem do repositório e colaboração.
- **Arquivos TXT** — Persistência e armazenamento dos registros.

> O projeto não utilizará banco de dados. O armazenamento será realizado exclusivamente por meio de arquivos de texto (`.txt`).

---

## 📁 Organização do Projeto

A estrutura de diretórios planejada é:

```text
BIBLIOTECA/
│
├── main.cpp
│
├── include/
│   ├── livro.h
│   ├── cliente.h
│   ├── administrador.h
│   ├── emprestimo.h
│   ├── ordenacao.h
│   ├── busca.h
│   └── arquivos.h
│
├── src/
│   ├── livro.cpp
│   ├── cliente.cpp
│   ├── administrador.cpp
│   ├── emprestimo.cpp
│   ├── ordenacao.cpp
│   ├── busca.cpp
│   └── arquivos.cpp
│
└── dados/
    ├── livros.txt
    ├── clientes.txt
    ├── administradores.txt
    └── emprestimos.txt
```

---

## 💾 Persistência de Dados

Os registros serão armazenados em arquivos de texto, separados de acordo com suas respectivas categorias:

- `livros.txt`
- `clientes.txt`
- `administradores.txt`
- `emprestimos.txt`

Ao iniciar o sistema, os dados serão carregados dos arquivos para a memória. Após operações de inclusão, alteração ou exclusão, os arquivos serão atualizados para preservar as informações.

---

## 👨‍💻 Desenvolvimento em Equipe

O projeto será desenvolvido colaborativamente por uma equipe de cinco integrantes, com responsabilidades divididas entre os módulos:

| Integrante | Responsabilidade |
|---|---|
| Pessoa 1 | Estrutura principal e acesso administrativo |
| Pessoa 2 | Gerenciamento de livros |
| Pessoa 3 | Gerenciamento de clientes |
| Pessoa 4 | Empréstimos e devoluções |
| Pessoa 5 | Persistência, ordenação, busca e consultas |

A integração dos módulos, os testes e a documentação serão realizados em conjunto.

---

## 📌 Contexto Acadêmico

Projeto interdisciplinar desenvolvido para integrar os conhecimentos das disciplinas de **Estrutura de Dados** e **Programação Orientada a Objetos**, colocando em prática conceitos estudados por meio da construção de um sistema funcional.

**Status:** 🚧 Em desenvolvimento.

---

## 📄 Licença

Projeto desenvolvido para fins acadêmicos e educacionais.
