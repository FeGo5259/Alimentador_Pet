document.addEventListener("DOMContentLoaded", function() {
    carregarHorarios();
});

// 🚀 Carregar lista de horários
function carregarHorarios() {
    fetch('/api/configuracoes/')
    .then(response => response.json())
    .then(data => {
        const lista = document.getElementById('lista-horarios');
        lista.innerHTML = '';
        data.forEach(item => {
            const li = document.createElement('li');
            li.classList.add('d-flex', 'justify-content-between', 'align-items-center', 'mb-2');
            li.innerHTML = `
                <span>${item.horario} - ${item.quantidade}g</span>
                <button class="btn btn-danger btn-sm" onclick="deletarHorario(${item.id})">🗑️</button>
            `;
            lista.appendChild(li);
        });
    });
}

// 🚀 Mostrar/ocultar formulário de adicionar horário
function toggleFormulario() {
    const form = document.getElementById('formulario-horario');
    form.style.display = (form.style.display === 'none' || form.style.display === '') ? 'block' : 'none';
}

// 🚀 Mostrar/ocultar campo de quantidade
function toggleQuantidade() {
    const quantidadeDiv = document.getElementById('form-quantidade');
    quantidadeDiv.style.display = (quantidadeDiv.style.display === 'none' || quantidadeDiv.style.display === '') ? 'block' : 'none';
}

// 🚀 Cancelar formulário
function cancelarFormulario() {
    document.getElementById('formulario-horario').style.display = 'none';
    document.getElementById('input-horario').value = '';
    document.getElementById('input-quantidade').value = '';
    document.getElementById('form-quantidade').style.display = 'none';
}

// 🚀 Salvar novo horário
function salvarHorario() {
    const horario = document.getElementById('input-horario').value;
    const quantidade = document.getElementById('input-quantidade').value;

    if (!horario) {
        alert('Por favor, informe o horário.');
        return;
    }
    if (!quantidade) {
        alert('Por favor, defina a quantidade de comida.');
        return;
    }

    // Salvar na API Django
    fetch('/api/configuracoes/', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({horario: horario, quantidade: quantidade})
    })
    .then(response => {
        if (response.ok) {
            alert('Horário adicionado com sucesso!');
            carregarHorarios();
            cancelarFormulario();
        } else {
            alert('Erro ao adicionar horário.');
        }
    });

    // Enviar também via Bluetooth
    enviarConfiguracoesBluetooth([horario], quantidade);
}


// 🚀 Deletar horário específico
function deletarHorario(id) {
    if (confirm('Tem certeza que deseja deletar este horário?')) {
        fetch('/api/configuracoes/', {
            method: 'DELETE',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ id: id })
        })
        .then(response => {
            if (response.ok) {
                alert('Horário deletado!');
                carregarHorarios();
                enviarHorariosParaESP32(); // 🔥 Envia lista atualizada via Bluetooth
            } else {
                alert('Erro ao deletar horário.');
            }
        });
    }
}

// 🚀 Função que envia a lista atualizada de horários + quantidade
function enviarHorariosParaESP32() {
    fetch('/api/configuracoes/')
    .then(response => response.json())
    .then(data => {
        const horarios = data.map(item => item.horario);
        const quantidade = data.length > 0 ? data[0].quantidade : 0;

        enviarConfiguracoesBluetooth(horarios, quantidade);
    });
}

// 🚀 Dispensar comida agora
function dispensar() {
     ipcRenderer.send('bt-dispensar');
    alert('Comando de dispensar enviado via Bluetooth.');
}


const { ipcRenderer } = require('electron');

function conectarBluetooth() {
    ipcRenderer.send('bt-connect');

    ipcRenderer.on('bt-status', (event, status) => {
        document.getElementById('status-bt').innerText = status;
    });
}

function enviarConfiguracoesBluetooth(horarios, quantidade) {
    const dados = { horarios, quantidade };
    ipcRenderer.send('bt-enviar-config', dados);
}

ipcRenderer.on('status-bt', (event, data) => {
    document.getElementById('temperatura').innerText = data.temperatura;
    document.getElementById('container').innerText = data.container;
});