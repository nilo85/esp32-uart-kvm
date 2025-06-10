import { LocationProvider, Router, Route, hydrate, prerender as ssr } from 'preact-iso';

import { Header } from './components/Header.jsx';
import { Terminal } from './pages/Terminal/index.jsx';
import { NotFound } from './pages/_404.jsx';
import './style.css';

export function App({numTerminals, host}) {
	return (
		<LocationProvider>
			<Header numTerminals={numTerminals} />
			<main>
				<Router>
					<Route host={host} path="/term/:channel/:id" component={Terminal} />
					<Route default component={NotFound} />
				</Router>
			</main>
		</LocationProvider>
	);
}

if (typeof window !== 'undefined') {
	const container = document.getElementById('app');
	const numTerminals = parseInt(container.dataset.numTerminals);
	const host = container.dataset.host;
	hydrate(<App host={host} numTerminals={numTerminals}/>, container);
}
