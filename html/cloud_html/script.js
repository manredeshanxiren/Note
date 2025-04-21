document.getElementById('uploadForm').addEventListener('submit', function(event) {
    event.preventDefault();
    
    let formData = new FormData();
    formData.append('file', document.getElementById('fileInput').files[0]);

    fetch('/upload', {
        method: 'POST',
        body: formData
    }).then(response => {
        if (response.status === 200) {
            alert('File uploaded successfully');
            loadFileList();
        } else {
            alert('File upload failed');
        }
    }).catch(error => {
        console.error('Error:', error);
    });
});

function loadFileList() {
    fetch('/listshow')
        .then(response => response.text())
        .then(html => {
            document.getElementById('fileList').innerHTML = html;
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

// Load file list on page load
window.onload = loadFileList;