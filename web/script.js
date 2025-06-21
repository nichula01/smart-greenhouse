const apiKey = "ea5047d4ed11275b728fca2970497c5c"; // Replace with your API Key
const city = "kandy"; // Change to your location
const currentWeatherUrl = `https://api.openweathermap.org/data/2.5/weather?q=${city}&units=metric&appid=${apiKey}`;
const forecastUrl = `https://api.openweathermap.org/data/2.5/forecast?q=${city}&units=metric&appid=${apiKey}`;

async function getWeather() {
    try {
        const response = await fetch(currentWeatherUrl);
        const data = await response.json();

        document.getElementById("location").textContent = `Location: ${data.name}`;
        document.getElementById("temp").textContent = `${data.main.temp}°C`;
        document.getElementById("humidity").textContent = `Humidity: ${data.main.humidity}%`;
        document.getElementById("wind").textContent = `Wind Speed: ${data.wind.speed} m/s`;
        document.getElementById("condition").textContent = `Condition: ${data.weather[0].description}`;

        // Additional details
        document.getElementById("pressure").textContent = `Pressure: ${data.main.pressure} hPa`;
        const sunrise = new Date(data.sys.sunrise * 1000).toLocaleTimeString();
        const sunset = new Date(data.sys.sunset * 1000).toLocaleTimeString();
        document.getElementById("sunrise").textContent = `Sunrise: ${sunrise}`;
        document.getElementById("sunset").textContent = `Sunset: ${sunset}`;

    } catch (error) {
        console.error("Error fetching weather data:", error);
        document.getElementById("location").textContent = "Error fetching weather data";
    }
}

async function getForecast() {
    try {
        const response = await fetch(forecastUrl);
        const data = await response.json();

        const forecastContainer = document.getElementById("forecast");
        forecastContainer.innerHTML = ""; // Clear previous content

        // Extract daily forecast data
        const dailyData = {};
        data.list.forEach((item) => {
            const date = item.dt_txt.split(" ")[0];
            if (!dailyData[date]) {
                dailyData[date] = item;
            }
        });

        // Display forecast with more details
        Object.values(dailyData).slice(0, 5).forEach((day) => {
            const card = document.createElement("div");
            card.classList.add("forecast-card");

            const date = new Date(day.dt_txt).toLocaleDateString();
            const temp = `${Math.round(day.main.temp)}°C`;
            const condition = day.weather[0].description;
            const icon = `https://openweathermap.org/img/wn/${day.weather[0].icon}.png`;
            const pressure = `Pressure: ${day.main.pressure} hPa`;
            const humidity = `Humidity: ${day.main.humidity}%`;
            const windSpeed = `Wind Speed: ${day.wind.speed} m/s`;

            card.innerHTML = `
                <p><strong>${date}</strong></p>
                <img src="${icon}" alt="${condition}">
                <p>${temp}</p>
                <p>${condition}</p>
                <p>${pressure}</p>
                <p>${humidity}</p>
                <p>${windSpeed}</p>
            `;

            forecastContainer.appendChild(card);
        });

    } catch (error) {
        console.error("Error fetching forecast data:", error);
    }
}

// Fetch weather data on page load
getWeather();
getForecast();
