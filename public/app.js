const API = "http://localhost:8080";

function loadItems() {
    fetch(`${API}/items`)
        .then(res => res.json())
        .then(data => {
            const table = document.getElementById("items");
            table.innerHTML = "";

            data.forEach(item => {
                table.innerHTML += `
                <tr>
                    <td>${item.id}</td>
                    <td>${item.name}</td>
                    <td>
                        <input type="number" value="${item.quantity}"
                            onchange="updateQty(${item.id}, this.value)">
                    </td>
                    <td>${item.price}</td>
                    <td>${item.threshold}</td>
                    <td>
                        <button onclick="deleteItem(${item.id})">❌</button>
                    </td>
                </tr>`;
            });
        });
}

function addItem() {
    const data = new URLSearchParams();
    data.append("name", document.getElementById("name").value);
    data.append("quantity", document.getElementById("quantity").value);
    data.append("price", document.getElementById("price").value);
    data.append("threshold", document.getElementById("threshold").value);

    fetch(`${API}/items`, {
        method: "POST",
        body: data
    }).then(() => loadItems());
}

function updateQty(id, qty) {
    const data = new URLSearchParams();
    data.append("quantity", qty);

    fetch(`${API}/items/${id}`, {
        method: "PUT",
        body: data
    });
}

function deleteItem(id) {
    fetch(`${API}/items/${id}`, {
        method: "DELETE"
    }).then(() => loadItems());
}

loadItems();
