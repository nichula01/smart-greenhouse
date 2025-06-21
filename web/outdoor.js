
const temperature = 28; // Celsius
const humidity = 55; // Percentage
const rainfall = 75; // mm


const tempCtx = document.getElementById('tempChart').getContext('2d');
new Chart(tempCtx, {
    type: 'line',
    data: {
        labels: ['10:00', '11:00', '12:00', '1:00', '2:00'],
        datasets: [{
            label: 'Temperature Over Time (°C)',
            data: [25, 26, 28, 30, 29],
            borderColor: '#ff0000',
            borderWidth: 2,
            fill: false,
        }]
    },
    options: {
        responsive: true,
    }
});


drawGauge('tempGauge', 'Temperature', temperature, 0, 50);


drawGauge('humidityGauge', 'Humidity', humidity, 0, 100);


drawGauge('rainfallGauge', 'Rainfall', rainfall, 0, 300);

function drawGauge(canvasId, label, value, min, max) {
    const ctx = document.getElementById(canvasId).getContext('2d');
    new Chart(ctx, {
        type: 'doughnut',
        data: {
            labels: [label],
            datasets: [{
                data: [value, max - value],
                backgroundColor: ['#008000', '#e0e0e0']
            }]
        },
        options: {
            circumference: 180,
            rotation: -90,
            cutoutPercentage: 70,
            plugins: {
                tooltip: {
                    enabled: false
                },
                legend: {
                    display: false
                }
            }
        }
    });
}
