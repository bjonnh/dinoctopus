import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
import { visualizer } from 'rollup-plugin-visualizer'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react(), visualizer()],
  server: {
    port: 5173,
    host: "0.0.0.0"
  },
  // This is valid for me publishing on the github pages, but you may want to change that
  base: '/dinoctopus/sysex-configurator/'
})
