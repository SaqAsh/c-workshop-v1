let currentSlide = 1;
const slideContainer = document.getElementById('slideContainer');

function bindEvents() {
    document.addEventListener('keydown', async (event) => await handleKeyPress(event));
    document.addEventListener('click', () => nextSlide());
}

async function handleKeyPress(event) {
    switch(event.key) {
        case 'ArrowLeft':
            await previousSlide();
            break;
        case 'ArrowRight':
        case ' ':
            await nextSlide();
            break;
    }
}

async function loadSlide() {
    try {
        const response = await fetch(`slides/${currentSlide.toString().padStart(2, '0')}.html`);
        if (response.status === 404) {
            return false; // Do nothing if the slide wasn't found
        }
        if (!response.ok) {
            throw new Error('Failed to load slide');
        }
        const html = await response.text();
        slideContainer.innerHTML = html;
    } catch (error) {
        console.error('Error loading slide:', error);
        slideContainer.innerHTML = '<p>Error loading slide</p>';
    }

    return true;
}

async function nextSlide() {
    currentSlide++;

    const loaded = await loadSlide();
    if (!loaded) {
        currentSlide--;
    }
}

async function previousSlide() {
    if (currentSlide > 1) {
        currentSlide--;

        const loaded = await loadSlide();
        if (!loaded) {
            currentSlide++;
        }
    }
}

bindEvents();
loadSlide();
