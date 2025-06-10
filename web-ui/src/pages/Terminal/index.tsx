import { useEffect, useRef } from 'preact/hooks';
import * as xterm from '@xterm/xterm';
import { FitAddon } from '@xterm/addon-fit';

import './style.css';
import '@xterm/xterm/css/xterm.css';


export function Terminal({host, channel, id}) {
	const terminalRef = useRef();
	useEffect(() => {
		const term = new xterm.Terminal();
		const fitAddon = new FitAddon();
		const resize = () => {
			fitAddon.fit();
		}

		term.loadAddon(fitAddon);
		term.open(terminalRef.current);
		resize();


		const formData = new FormData();
		formData.append("channel", channel);
		formData.append("terminal", id);
		
		//TODO implement proper handlnig of promise
		fetch("http://"+host+"/set", {
			method: "POST",
			body: formData
		});

		term.write("Connecting...");

        var ws = new WebSocket('ws://'+host+'/ws_' + channel + '?terminal=' + id);

        ws.onopen = function() {
			term.writeln(" OK");
        };
        ws.onmessage = async function(event) {
            if (event.type == "message" && typeof(event.data) == "string") {
                term.writeln(event.data);
            } else {
                term.write(new Uint8Array(await event.data.arrayBuffer()));
            }
        };
		term.onData(data => {
          ws.send(data);
        })
        ws.onclose = function() {
            term.writeln("connection closed")
        };
        ws.onerror = function(error) {
            term.writeln("ERROR: " + error)
        };
		window.addEventListener('resize', resize);


		return function cleanup() {
			ws.close();
			term.dispose();
			window.removeEventListener('resize', resize);
		}
	});
	return (
		<div class="terminal" ref={terminalRef}></div>
	);
}


