import { useLocation, useRoute } from 'preact-iso';

const channelPattern = new RegExp('^/term/([0-9]+)/.*$')

export function Header({numTerminals}) {
	const { url, query, path } = useLocation();
	const channel = channelPattern.exec(path)[1];
	return (
		<header>
			<span class="title">ESP32-UART-KVM</span>
			<nav>
				{[...Array(numTerminals)].map((x, i) => ({ url: '/term/' + channel + '/' + i, id: i})).map((e) =>
					<a href={e.url} class={url == e.url && 'active'}>
						{e.id}
					</a>
				)}
			</nav>
		</header>
	);
}
