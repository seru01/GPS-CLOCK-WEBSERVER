/***********************
 * CLOCK CONTENTS
 ***********************/
// GPS-CLOCK
async function getgpsClock() {
  try {
    const response = await fetch("./gpsClock.json");
    if (!response.ok) {
      throw new Error(`HTTP error! Status: ${response.status}`);
    }
    return await response.json();
  } catch (error) {
    console.error("Unable to fetch data:", error);
    throw error;
  }
}

// DISPLAY THE GPS-CLOCK DATA
async function displayGpsClock() {
  try {
    const updateClock = async () => {
      const data = await getgpsClock();
      const { hour, minute, second } = data;
      const GPSdata = `${hour.toString().padStart(2, "0")}:${minute
        .toString()
        .padStart(2, "0")}:${second.toString().padStart(2, "0")}`;
      document.getElementById("TimeDisplayValue").textContent = GPSdata;
    };
    // Update the clock immediately upon loading
    await updateClock();
    // Update the clock every second
    setInterval(updateClock, 1000);
  } catch (error) {
    console.error("Error displaying value:", error);
  }
}
displayGpsClock();
